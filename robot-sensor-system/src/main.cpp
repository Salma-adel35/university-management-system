#include "../include/sensor.h"
#include "../include/utils.h"
#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

// 1. تعريف فئة مشتقة وهمية (Mock Sensor)
// هذه الفئة لا تحتاج لملفات Headers أخرى.
class MockSensor : public Sensor {
public:
    // نستخدم مُنشئ Sensor المُعدَّل (بواسطة const& string)
    MockSensor(int id) : Sensor(id, "Mock Sensor", "TestType", "Units") {}

    // تطبيق الدوال الافتراضية النقية
    // لاحظي أننا نستخدم المتغيرات المحمية بالشرطة السفلية: status_ و lastReading_
    double readData() override {
        lastReading_ = 42.0; 
        status_ = SensorStatus::OK;
        return lastReading_;
    }
    void calibrate() override {
        cout << "[Mock] Sensor ID=" << id_ << " calibrated successfully." << endl;
    }
};


void test_utils() {
    cout << "--- Testing Utils Module ---\n";

    // اختبار توليد الـ ID
    int id1 = Utils::generateID();
    int id2 = Utils::generateID();
    cout << "Generated IDs: " << id1 << ", " << id2 << endl;
    if (id2 == id1 + 1) {
        cout << "✅ ID Generation: OK\n";
    } else {
        cout << "❌ ID Generation: FAILED\n";
    }

    // اختبار توليد الأرقام العشوائية
    double min = 10.0;
    double max = 20.0;
    double rand_val = Utils::randomDouble(min, max);
    cout << "Random Double (" << min << "-" << max << "): " << rand_val << endl;
    if (rand_val >= min && rand_val <= max) {
        cout << "✅ Random Double: OK\n";
    } else {
        cout << "❌ Random Double: FAILED\n";
    }

    cout << "Current DateTime: " << Utils::currentDateTime() << endl;
    cout << "----------------------------\n";
}

void test_sensor_core() {
    cout << "--- Testing Sensor Core and Polymorphism ---\n";
    
    // إنشاء مؤشر ذكي (std::shared_ptr) يشير إلى الفئة المشتقة
    auto mock_ptr = std::make_shared<MockSensor>(Utils::generateID());
    
    if (mock_ptr) {
        cout << "✅ Smart Pointer Creation: OK\n";
    } else {
        cout << "❌ Smart Pointer Creation: FAILED\n";
        return;
    }

    // اختبار التعدد الشكلي واستدعاء الدوال
    mock_ptr->readData();
    cout << "Info Check: ";
    mock_ptr->printInfo(); 

    if (mock_ptr->getLastReading() == 42.0 && mock_ptr->getStatus() == SensorStatus::OK) {
        cout << "✅ Polymorphism (readData/printInfo): OK\n";
    } else {
        cout << "❌ Polymorphism: FAILED\n";
    }
    
    // اختبار دالة الفشل
    mock_ptr->setFaulty(true);
    if (mock_ptr->isFaulty()) {
        cout << "✅ setFaulty: OK\n";
        mock_ptr->calibrate(); // اختبار دالة calibrate
    } else {
        cout << "❌ setFaulty: FAILED\n";
    }

    cout << "------------------------------------------\n";
}

int main() {
    // تشغيل دوال الاختبار
    test_utils();
    test_sensor_core();
    return 0;
}