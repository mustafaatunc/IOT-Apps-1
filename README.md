# ESP32 Çevre İzleme ve Uyarı Sistemi

Bu proje, ESP32 mikrodenetleyicisi kullanılarak geliştirilmiş bir Nesnelerin İnterneti (IoT) tabanlı çevre izleme sistemidir. [cite_start]Sistem; ortamdaki sıcaklık, nem ve ışık şiddetini ölçerek bu verileri bir I2C LCD ekran üzerinde düzenli aralıklarla gösterir ve ortam koşullarına göre RGB LED ile görsel uyarılar üretir[cite: 52, 56, 62, 67, 68, 69, 70]. [cite_start]Projenin simülasyonu Wokwi ortamında gerçekleştirilmiştir[cite: 1].

## 🚀 Özellikler

* [cite_start]**Eşzamanlı Sensör Okuma:** Sensör verileri her 2000 milisaniyede bir `millis()` kullanılarak, mikrodenetleyiciyi duraklatmadan (non-blocking) okunur[cite: 30].
* [cite_start]**Round-Robin Ekran Yönetimi:** Ölçülen Nem, Sıcaklık ve Işık değerleri sırayla LCD ekranda gösterilir[cite: 55, 61, 65].
* [cite_start]**Dinamik Ekran Süresi:** Sisteme entegre edilen bir buton yardımıyla LCD ekranın veri gösterim süresi 2000 ms ile 4000 ms arasında dinamik olarak değiştirilebilir[cite: 12, 40].
* [cite_start]**Akıllı Durum Bildirim LED'i:** Ortamın aydınlık durumuna ve sıcaklık seviyesine göre RGB LED farklı renklerde yanarak durumu bildirir[cite: 67, 68, 69, 70].
* [cite_start]**Seri Port Gözlemi:** Okunan sensör verileri her 3000 ms'de bir 115200 baud hızında seri monitöre aktarılır[cite: 18, 46].

## 🛠️ Kullanılan Donanımlar

* ESP32 DevKit V4
* [cite_start]DHT22 Sıcaklık ve Nem Sensörü [cite: 3]
* [cite_start]LDR (Foto Direnç) Işık Sensörü [cite: 3]
* [cite_start]16x2 I2C Destekli LCD Ekran [cite: 6]
* RGB LED (Ortak Katot) ve 3 adet 220Ω Direnç
* 1 Adet Push Buton

## 🔌 Pin Bağlantıları

Kablolama işlemleri Wokwi projesine uygun olarak aşağıdaki şekilde yapılandırılmıştır:

| Bileşen | ESP32 Pini | Açıklama |
| :--- | :--- | :--- |
| **DHT22 (SDA)** | `Pin 15` | [cite_start]Sıcaklık ve Nem verisi [cite: 3] |
| **LDR (AO)** | `Pin 34` | [cite_start]Analog Işık verisi [cite: 3] |
| **Buton** | `Pin 18` | [cite_start]Ekran süresi değiştirme [cite: 3] |
| **LCD (SDA)** | `Pin 21` | [cite_start]I2C Veri Hattı [cite: 25] |
| **LCD (SCL)** | `Pin 22` | [cite_start]I2C Saat Hattı [cite: 25] |
| **RGB (Kırmızı)** | `Pin 25` | [cite_start]PWM Çıkışı [cite: 4] |
| **RGB (Yeşil)** | `Pin 26` | [cite_start]PWM Çıkışı [cite: 4] |
| **RGB (Mavi)** | `Pin 27` | [cite_start]PWM Çıkışı [cite: 4] |

## 📊 Karar Algoritmaları ve Bildirimler

### LCD Ekran Uyarı Durumları
* [cite_start]**Nem:** `%60` ve altı `LOW`, üzeri `HIGH` olarak nitelendirilir[cite: 54].
* [cite_start]**Sıcaklık:** `0°C` altı `VERY LOW`, `0-30°C` arası `NORMAL`, `30°C` üzeri `HIGH` olarak ekrana yansır[cite: 58, 59, 60].
* [cite_start]**Işık:** `10 Lux` altı `KARANLIK`, üzeri `AYDINLIK` olarak değerlendirilir[cite: 64].

### RGB LED Uyarı Renkleri
LED renkleri çevresel duruma göre şu şekilde tepki verir:

| Koşul | LED Rengi | RGB Değeri (R, G, B) |
| :--- | :--- | :--- |
| Işık Şiddeti < 10 Lux | Mavi | [cite_start]`(0, 0, 255)` [cite: 67] |
| Aydınlık ve Sıcaklık < 0°C | Yeşil | [cite_start]`(0, 255, 0)` [cite: 68] |
| Aydınlık ve 0°C <= Sıcaklık <= 30°C | Turuncu | [cite_start]`(255, 165, 0)` [cite: 69] |
| Aydınlık ve Sıcaklık > 30°C | Kırmızı | [cite_start]`(255, 0, 0)` [cite: 70] |

## ⚙️ Kurulum ve Çalıştırma

1. Bu depoyu yerel bilgisayarınıza klonlayın.
2. Arduino IDE veya PlatformIO (VS Code) üzerinden `sketch.ino` dosyasını açın.
3. [cite_start]Projenin düzgün derlenebilmesi için `LiquidCrystal I2C` ve `DHT sensor library` kütüphanelerini kurduğunuzdan emin olun[cite: 72].
4. ESP32 kartınızı bağlayın ve kodu yükleyin.
5. [cite_start]Seri Port ekranını `115200` baud hızında açarak [cite: 18] arka plan verilerini gözlemlemeye başlayın.

## 📄 Lisans

Bu proje MIT Lisansı altında lisanslanmıştır. Kodları inceleyebilir, geliştirebilir ve kendi projelerinizde özgürce kullanabilirsiniz.
