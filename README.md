# ESP32 Çevre İzleme ve Uyarı Sistemi

Bu proje, ESP32 mikrodenetleyicisi kullanılarak geliştirilmiş bir Nesnelerin İnterneti (IoT) tabanlı çevre izleme sistemidir. Sistem; ortamdaki sıcaklık, nem ve ışık şiddetini ölçerek bu verileri bir I2C LCD ekran üzerinde düzenli aralıklarla gösterir ve ortam koşullarına göre RGB LED ile görsel uyarılar üretir. Projenin simülasyonu Wokwi ortamında gerçekleştirilmiştir.

## 🚀 Özellikler

* **Eşzamanlı Sensör Okuma:** Sensör verileri her 2000 milisaniyede bir `millis()` kullanılarak, mikrodenetleyiciyi duraklatmadan (non-blocking) okunur.
* **Round-Robin Ekran Yönetimi:** Ölçülen Nem, Sıcaklık ve Işık değerleri sırayla LCD ekranda gösterilir.
* **Dinamik Ekran Süresi:** Sisteme entegre edilen bir buton yardımıyla LCD ekranın veri gösterim süresi 2000 ms ile 4000 ms arasında dinamik olarak değiştirilebilir.
* **Akıllı Durum Bildirim LED'i:** Ortamın aydınlık durumuna ve sıcaklık seviyesine göre RGB LED farklı renklerde yanarak durumu bildirir.
* **Seri Port Gözlemi:** Okunan sensör verileri her 3000 ms'de bir 115200 baud hızında seri monitöre aktarılır.

## 🛠️ Kullanılan Donanımlar

* ESP32 DevKit V4
* DHT22 Sıcaklık ve Nem Sensörü
* LDR (Foto Direnç) Işık Sensörü 
* 16x2 I2C Destekli LCD Ekran 
* RGB LED (Ortak Katot) ve 3 adet 220Ω Direnç
* 1 Adet Push Buton

## 🔌 Pin Bağlantıları

Kablolama işlemleri Wokwi projesine uygun olarak aşağıdaki şekilde yapılandırılmıştır:

| Bileşen | ESP32 Pini | Açıklama |
| :--- | :--- | :--- |
| **DHT22 (SDA)** | `Pin 15` | Sıcaklık ve Nem verisi |
| **LDR (AO)** | `Pin 34` | Analog Işık verisi |
| **Buton** | `Pin 18` | Ekran süresi değiştirme |
| **LCD (SDA)** | `Pin 21` | I2C Veri Hattı |
| **LCD (SCL)** | `Pin 22` | I2C Saat Hattı |
| **RGB (Kırmızı)** | `Pin 25` | PWM Çıkışı |
| **RGB (Yeşil)** | `Pin 26` | PWM Çıkışı |
| **RGB (Mavi)** | `Pin 27` | PWM Çıkışı |

## 📊 Karar Algoritmaları ve Bildirimler

### LCD Ekran Uyarı Durumları

* **Nem:** `%60` ve altı `LOW`, üzeri `HIGH` olarak nitelendirilir.
* **Sıcaklık:** `0°C` altı `VERY LOW`, `0-30°C` arası `NORMAL`, `30°C` üzeri `HIGH` olarak ekrana yansır.
* **Işık:** `10 Lux` altı `KARANLIK`, üzeri `AYDINLIK` olarak değerlendirilir.

### RGB LED Uyarı Renkleri

LED renkleri çevresel duruma göre şu şekilde tepki verir:

| Koşul | LED Rengi | RGB Değeri (R, G, B) |
| :--- | :--- | :--- |
| Işık Şiddeti < 10 Lux | Mavi | `(0, 0, 255)` |
| Aydınlık ve Sıcaklık < 0°C | Yeşil | `(0, 255, 0)` |
| Aydınlık ve 0°C <= Sıcaklık <= 30°C | Turuncu | `(255, 165, 0)` |
| Aydınlık ve Sıcaklık > 30°C | Kırmızı | `(255, 0, 0)` |

## ⚙️ Kurulum ve Çalıştırma

1. Bu depoyu yerel bilgisayarınıza klonlayın.
2. Arduino IDE veya PlatformIO (VS Code) üzerinden `sketch.ino` dosyasını açın.
3. Projenin düzgün derlenebilmesi için `LiquidCrystal I2C` ve `DHT sensor library` kütüphanelerini kurduğunuzdan emin olun.
4. ESP32 kartınızı bağlayın ve kodu yükleyin.
5. Seri Port ekranını `115200` baud hızında açarak arka plan verilerini gözlemlemeye başlayın.

