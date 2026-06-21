#include <Wire.h>                      // I2C haberleşmesi için gerekli Wire kütüphanesi eklendi
#include <LiquidCrystal_I2C.h>         // I2C destekli LCD ekranı kontrol etmek için kütüphane eklendi
#include <DHT.h>                       // DHT sıcaklık ve nem sensörü için kütüphane eklendi

#define DHTPIN 15                      // DHT22 sensörünün veri pini ESP32'nin 15 numaralı pinine atandı
#define DHTTYPE DHT22                  // Kullanılan sensörün modeli DHT22 olarak belirlendi
#define LDR_PIN 34                     // LDR (Işık) sensörünün analog okuma pini 34 olarak atandı
#define BUTTON_PIN 18                  // Süre değiştirme butonunun pini 18 olarak atandı
#define LED_R 25                       // RGB LED'in Kırmızı (Red) bacağı 25 numaralı pine atandı
#define LED_G 26                       // RGB LED'in Yeşil (Green) bacağı 26 numaralı pine atandı
#define LED_B 27                       // RGB LED'in Mavi (Blue) bacağı 27 numaralı pine atandı

DHT dht(DHTPIN, DHTTYPE);              // Belirlenen pin ve sensör tipine göre dht nesnesi oluşturuldu
LiquidCrystal_I2C lcd(0x27, 16, 2);    // 0x27 adresli, 16 sütun ve 2 satırlı LCD nesnesi tanımlandı

const float GAMMA = 0.7;               // LDR'nin lux hesaplaması için gerekli olan GAMMA sabiti tanımlandı
const float RL10 = 50;                 // LDR'nin 10 lux altındaki direnç sabiti tanımlandı

unsigned long oncekiZamanLCD = 0;      // LCD ekranın son güncellenme zamanını tutacak değişken oluşturuldu
unsigned long oncekiZamanSeri = 0;     // Seri monitöre son yazdırma zamanını tutacak değişken oluşturuldu
unsigned long oncekiZamanSens = 0;     // Sensörlerin son okunma zamanını tutacak değişken oluşturuldu
int ekranSuresi = 2000;                // Varsayılan LCD ekran gösterim süresi 2000 ms (2 saniye) olarak ayarlandı
int ekranSirasi = 0;                   // Round Robin algoritması için gösterilecek ekranın sırasını tutan değişken
bool sonButonDurumu = HIGH;            // Butonun bir önceki durumunu tutacak değişken (Pull-up olduğu için HIGH)

float nem = 0.0;                       // Ölçülen nem değerini tutacak ondalıklı değişken tanımlandı
float sicaklik = 0.0;                  // Ölçülen sıcaklık değerini tutacak ondalıklı değişken tanımlandı
float lux = 0.0;                       // Ölçülen ve hesaplanan ışık şiddetini tutacak ondalıklı değişken tanımlandı

void setup() {                         // Sistemin ilk açılışında bir kez çalışacak ayar fonksiyonu
  Serial.begin(115200);                // Seri monitör haberleşmesi 115200 baud hızında başlatıldı
  
  pinMode(LED_R, OUTPUT);              // Kırmızı LED pini karta ÇIKIŞ olarak ayarlandı
  pinMode(LED_G, OUTPUT);              // Yeşil LED pini karta ÇIKIŞ olarak ayarlandı
  pinMode(LED_B, OUTPUT);              // Mavi LED pini karta ÇIKIŞ olarak ayarlandı
  
  analogWrite(LED_R, 255); delay(300); analogWrite(LED_R, 0); // Başlangıçta Kırmızı LED test edildi
  analogWrite(LED_G, 255); delay(300); analogWrite(LED_G, 0); // Başlangıçta Yeşil LED test edildi
  analogWrite(LED_B, 255); delay(300); analogWrite(LED_B, 0); // Başlangıçta Mavi LED test edildi

  Wire.begin(21, 22);                  // ESP32 için I2C haberleşmesi SDA(21) ve SCL(22) pinleriyle başlatıldı
  lcd.init();                          // LCD ekranın arka plan başlatma işlemleri yapıldı
  lcd.backlight();                     // LCD ekranın arka plan aydınlatması (ışığı) açıldı
  
  dht.begin();                         // DHT22 sıcaklık ve nem sensörü başlatıldı
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // Buton pini dahili direnç (Pull-Up) ile GİRİŞ olarak ayarlandı
}                                      // Setup fonksiyonu sonlandırıldı

void loop() {                          // Sistemin gücü kesilene kadar sürekli çalışacak sonsuz döngü
  unsigned long simdikiZaman = millis(); // Cihazın çalışma süresi milisaniye cinsinden alındı

  if (simdikiZaman - oncekiZamanSens >= 2000 || oncekiZamanSens == 0) { // Sensörler için 2 saniye geçtiyse
    oncekiZamanSens = simdikiZaman;    // Bir sonraki sensör ölçümü için zamanlayıcı güncellendi
    float h = dht.readHumidity();      // DHT22 üzerinden güncel nem değeri okundu
    float t = dht.readTemperature();   // DHT22 üzerinden güncel sıcaklık değeri okundu
    
    if (!isnan(h) && !isnan(t)) {      // Eğer okunan değerler NaN (hatalı) DEĞİLSE
      nem = h;                         // Geçerli nem değeri ana değişkene kaydedildi
      sicaklik = t;                    // Geçerli sıcaklık değeri ana değişkene kaydedildi
    }                                  // DHT22 doğrulama bloğu kapatıldı
    
    int analog = analogRead(LDR_PIN);  // LDR üzerinden 0-4095 arası ham analog sinyal okundu
    if (analog > 0) {                  // Eğer analog sinyal 0'dan büyükse hesaplamaya başla
      float v = analog / 4095.0 * 3.3; // Ham değer ESP32'nin 3.3V referansına göre gerilime çevrildi
      if (v < 3.3) {                   // Sıfıra bölünme hatasını önlemek için voltaj kontrol edildi
        float r = 2000 * v / (1 - v / 3.3); // Gerilim bölücü formülüyle LDR direnci hesaplandı
        lux = pow(RL10 * 1e3 * pow(10, GAMMA) / r, (1 / GAMMA)); // Lux değeri formülle hesaplandı
      }                                // Voltaj kontrol bloğu kapatıldı
    }                                  // Analog okuma kontrol bloğu kapatıldı
  }                                    // Sensör okuma zamanlayıcı bloğu kapatıldı

  bool buton = digitalRead(BUTTON_PIN);// Butonun o anki anlık basılma durumu okundu
  if (buton == LOW && sonButonDurumu == HIGH) { // Eğer butona yeni basıldıysa (Düşen kenar tetiklemesi)
    ekranSuresi = (ekranSuresi == 2000) ? 4000 : 2000; // Süre 2 sn ise 4 sn yap, 4 ise 2 sn yap
    Serial.print("LCD suresi degistirildi: "); // Sürenin değiştiğine dair mesaj seri monitöre yazdırıldı
    Serial.print(ekranSuresi / 1000);  // Yeni süre saniye cinsine çevrilerek yazdırıldı
    Serial.println(" sn");             // Mesajın sonuna "sn" birimi eklendi ve alt satıra geçildi
    delay(200);                        // Buton titremelerini önlemek için kısa bekleme eklendi
  }                                    // Buton kontrol if bloğu kapatıldı
  sonButonDurumu = buton;              // Döngü sonunda butonun son durumu bir sonraki tur için kaydedildi

  if (simdikiZaman - oncekiZamanSeri >= 3000 || oncekiZamanSeri == 0) { // Seri yazdırma için 3 saniye geçtiyse
    oncekiZamanSeri = simdikiZaman;    // Bir sonraki seri yazdırma için zamanlayıcı güncellendi
    Serial.print("Nem: "); Serial.print(nem); Serial.print("% || "); // Nem verisi formata uygun yazdırıldı
    Serial.print("Sicaklik: "); Serial.print(sicaklik); Serial.print(" C || "); // Sıcaklık verisi yazdırıldı
    Serial.print("Isik: "); Serial.print((int)lux); Serial.println(" lux"); // Lux değeri yazdırılıp alt satıra geçildi
  }                                    // Seri monitör yazdırma bloğu kapatıldı

  if (simdikiZaman - oncekiZamanLCD >= ekranSuresi || oncekiZamanLCD == 0) { // LCD ekran süresi (2 veya 4 sn) dolduysa
    oncekiZamanLCD = simdikiZaman;     // Bir sonraki ekran değişimi için zamanlayıcı güncellendi
    lcd.clear();                       // Bir önceki yazılar birbirine karışmasın diye ekran tamamen temizlendi
    
    if (ekranSirasi == 0) {            // Round Robin Algoritması 1. Adım: NEM EKRANI
      lcd.print("NEM            "); lcd.setCursor(15,0); lcd.print(ekranSuresi/1000); // Üst satıra NEM ve süre yazdırıldı
      lcd.setCursor(0,1); lcd.print(nem); lcd.print("% "); // İkinci satıra nemin sayısal değeri yazdırıldı
      if (nem <= 60) lcd.print("LOW"); else lcd.print("HIGH"); // Nem değerine göre LOW veya HIGH mesajı yazdırıldı
      ekranSirasi = 1;                 // Bir sonraki ekrana geçiş için sıra 1 yapıldı
    } else if (ekranSirasi == 1) {     // Round Robin Algoritması 2. Adım: SICAKLIK EKRANI
      lcd.print("SICAKLIK       "); lcd.setCursor(15,0); lcd.print(ekranSuresi/1000); // Üst satıra SICAKLIK ve süre yazdırıldı
      lcd.setCursor(0,1); lcd.print(sicaklik); lcd.print("C "); // İkinci satıra sıcaklığın sayısal değeri yazdırıldı
      if (sicaklik < 0) lcd.print("VERY LOW"); // Sıcaklık 0'dan küçükse VERY LOW yazdırıldı
      else if (sicaklik <= 30) lcd.print("NORMAL"); // Sıcaklık 0-30 arasındaysa NORMAL yazdırıldı
      else lcd.print("HIGH");          // Sıcaklık 30'dan büyükse HIGH yazdırıldı
      ekranSirasi = 2;                 // Bir sonraki ekrana geçiş için sıra 2 yapıldı
    } else {                           // Round Robin Algoritması 3. Adım: IŞIK EKRANI
      lcd.print("ISIK           "); lcd.setCursor(15,0); lcd.print(ekranSuresi/1000); // Üst satıra ISIK ve süre yazdırıldı
      lcd.setCursor(0,1);              // İkinci satırın başına geçildi
      if (lux < 10) lcd.print("KARANLIK"); else lcd.print("AYDINLIK"); // Lux'a göre KARANLIK veya AYDINLIK yazdırıldı
      ekranSirasi = 0;                 // Döngüyü başa sarmak için sıra tekrar 0'a (NEM ekranına) çekildi
    }                                  // Ekran sıra seçimi (if-else if-else) bloğu kapatıldı
  }                                    // LCD ekran güncelleyici Round Robin bloğu kapatıldı

  if (lux < 10) {                      // Eğer LDR değeri 10 lux altındaysa (KARANLIK durumu)
    analogWrite(LED_R, 0); analogWrite(LED_G, 0); analogWrite(LED_B, 255); // Ortam karanlıksa sadece Mavi renk yak
  } else {                             // Eğer ortam 10 lux veya üzerindeyse (AYDINLIK durumu)
    if (sicaklik < 0) {                // Eğer sıcaklık VERY LOW değerindeyse
      analogWrite(LED_R, 0); analogWrite(LED_G, 255); analogWrite(LED_B, 0); // YEŞİL 
    } else if (sicaklik <= 30) {       // Eğer sıcaklık NORMAL aralığındaysa
      analogWrite(LED_R, 255); analogWrite(LED_G, 165); analogWrite(LED_B, 0); // TURUNCU
    } else {                           // Eğer sıcaklık HIGH seviyesindeyse (30 üzeri)
      analogWrite(LED_R, 255); analogWrite(LED_G, 0); analogWrite(LED_B, 0); // KIRMIZI 
    }                                  // Sıcaklığa göre renk belirleme bloğu kapatıldı
  }                                    // Ana aydınlık/karanlık kontrol bloğu kapatıldı
}                                      // Sonsuz döngü (loop) sonlandırıldı