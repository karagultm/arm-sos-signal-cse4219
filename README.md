# arm-sos-signal

## Yaptıklarımız: 

Main_basic.c dosyasında main fonksiyonu içersinde enable_HSI() ile configure_LED_pin() fıonksyionları hazır olarak verilmişti. Biz de button a basıldığında led in yanmasını kontrol edebilemek için configure_LED_pin() fonksiyonundan yararlanarak cofigure_BUTTON_pin() fonksiyonunu yazdık.

### configure_BUTTON_pin()
Bu fonksiyonda öncelikle GPIO da C portunda butonumuz bulunduğu için C portunun clock unu aktif ettik. C portunun moderını input olarak ve pupdr modulunu de no pullup no pull down olarak ayarladık bizden istenildiği üzere.

### SysTick_Initialize()

Bu fonksiyon ile yaptığımız olay aslında şu. Bizim kartımızın frekansını 16Mhz olarak bildiğimizden ve her bir interruptın arasındaki sürerin 1ms olmasının bizim işimizi kolaylaştıracağını düşündüğümüzden interruplar arasında 1ms olacak şekilde 16k tick verdik. İçerisindeki kısımları derste gördüğümüz slide üzerinden almıştık. Burada syystick enable yapıldığında systick handler otomatik olarak çalışmaya başlar.

İki interrupt arası formülü: 
- tick = saniye x Herz
- reload = tick - 1 
- tick = 1.10^-3 (kullanım kolaylığı için 1ms i tercih ettik) x 16.10^6 (Clok'umuzun Herz i) = 16000

### systick_handler
bu fonksiyon bir interrup fonksiyonu ve systick initialize de her bir interrupt arasını 1ms olacak şekilde tick değerini 16kya ayarladığımızdan bu kod da 1ms de bir gerçekleşir ve timedelay sıfırdan büyükse timedelay i azaltır.

### while loop

Bu loop ile birlikte aslında kartımızın sürekli olarak input okumasını sağlıyoruz. 

### Take input()

Button pin değerimiz 13 oalrak default değer. 13 kere sola shift edip hedef bölge ile end yapıyoruz. Eğer input varsa IDR kısmı sıfır olacak ve parantez arası 0 olacak bunu 1 e çevirip true döndürcez. 

### blink()

iki arguman alıyoruz. birinci argumanımız bizim nekadar süre delay yapacağımız ikinci argumanımız ise ışığı nekadar yanık tutacağımızı gösteriyor. içindeki toglle led zaten default olarak vardı.  

### delay()
bu kod timedelay e bir değer atar. örneğin biz 250  atıyoruz. arka planda systick handler otomatik çalışıyor bunu biliyoruz. ve 1ms de bir gerçeklişiyor bu durum. while loop içinde timedelay sıfıra eşit olanakadar bekliyoruz. systick handler 250 yi sıfıra düşürüne kadar her bir 1ms 1 düşürüyor ve biz while loop da 250 ms beklemiş oluyoruz. sıfıra eşit olunca while looptan çıkıyoruz ve delay fonksiyonumuzu tamamlıyoruz. 
