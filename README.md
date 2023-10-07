# ONEMLİ ACIKLAMA
1) Proje indirildiğinde proje ismi "MonitoringProject-master" olarak iniyor bu sebeple proje dosyasının adını "Monitoring" yapınız.
2) Daha sonra runBoth.Bat'i çalıştırınız.
   
## MonitoringProject

### 1)Proje için gerekli olan mimarinin nasıl gerçeklendiği 
---------------------------------------------------------------
Proje için gerekli olan mimari, C++ ve C# programlarının bir arada çalıştığı bir sistemdir. 
C++ programı için visual studio'da bulunan c++ empty console application, c# programı için windows forms application kullanıldı.
C++ programı, sistem bilgilerini okurken; C# programı, bu bilgileri düzenleyerek bir kullanıcı arayüzü üzerinde görselleştirir.
Bu, iki ayrı dilde yazılmış iki ayrı programın işbirliği yaparak çalıştığı bir mimariyi temsil eder.


### 2)Hangi teknolojilerin kullanılması gerektiği  
-----------------------------------------------------
Dil olarak;
C++: Sistem bilgilerini okumak için C++ kullanılmıştır. 
C++ kullanmanın nedeni, sistem düzeyinde işlem yapabilme yeteneğine sahip olmasıdır.

C#: Kullanıcı arayüzü oluşturmak ve sistem bilgilerini görselleştirmek için C# kullanılmıştır. 
C# tercih edilmesinin nedeni, Windows tabanlı uygulamaları hızlı bir şekilde geliştirmeye uygun olmasıdır.

IDE olarak;
Visual Studio kullanılmıştır.


### 3)Sistem bilgilerinin alınmasında kullanılması gereken fonksiyonlar, sınıflar, kütüphaneler 
------------------------------------------------------------------------------------
C++ kısmında Windows API'leri ve ilgili kütüphaneler kullanılmıştır. 
Örneğin, "GetDiskFreeSpaceEx" ve "GetSystemTimes" gibi API'ler sistem bilgilerini okumak için kullanılmıştır.
C# kısmında Windows Forms kullanılmıştır. 
Kullanıcı arayüzü oluşturmak için "System.Windows.Forms" kütüphanesi kullanılmıştır.


### 4)C# ve C++ kodları arasındaki veri transferi için hangi yöntemlerin kullanıldığı 
------------------------------------------------------------------------------------
C# ve C++ kodları arasındaki veri transferi için:
C++ kodu, sistem bilgilerini bir dosyaya(output.txt) belirli aralıklarla (30 saniyede 1) yazmaktadır.
C# kodu, belirli aralıklarla bu dosyayı okuyarak verileri alıp kullanmaktadır. 
Bu, veri transferi için basit bir dosya tabanlı iletişim yöntemidir.


### 5)Projede kullanılan sınıfların, fonksiyonların OOP yapına uygunluğu 
-------------------------------------------------------------------------
C++ kısmında SystemInfo adlı bir sınıf kullanılmıştır. 
Bu sınıf, sistem bilgilerini çeşitli metotlar aracılığıyla döndürerek OOP prensiplerine uygun bir şekilde organize etmiştir.
C# kısmında da sınıf yapısı kullanılmıştır. ClassCPU, ClassRAM, ve ClassHDD gibi sınıflar, verileri düzenlemek ve daha iyi organizasyon sağlamak için kullanılmıştır.


### 6)Projelerdeki yazılım standartlarına uyum (sınıfların, fonksiyonların, değişkenlerin isimlendirmesi gibi)
------------------------------------------------------------------------------------
Sınıf, fonksiyon ve değişken isimlendirmeleri açık ve anlaşılır bir şekilde yazılmıştır.
Kod içindeki yorum satırları, kodun anlaşılmasını kolaylaştırmak için eklenmiştir.
Proje içinde uygun hata yakalama ve raporlama mekanizmaları kullanılmıştır (örneğin, dosya açma hatası raporu).
C# kodu, Windows Forms uygulama yapısına uygun bir şekilde tasarlanmıştır.

