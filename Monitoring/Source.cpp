#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <wbemidl.h>
#include <comdef.h>

class SystemInfo {
public:

    //Sistemin HDD'sindeki bo� alan� GB cinsinden d�nd�ren fonksiyon 
    double GetHDDFreeSpaceGB(const std::wstring& drive) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            return static_cast<double>(freeBytesAvailable.QuadPart) / (1024 * 1024 * 1024); //GB'a d�nd�r�r
        }

        return -1.0; 
    }

    //sistemin HDD'sindeki b�t�n alan� GB cinsinden d�nd�ren fonksiyon 
    double GetHDDFullSpaceGB(const std::wstring& drive) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            return static_cast<double>(totalNumberOfBytes.QuadPart) / (1024 * 1024 * 1024); //GB'a d�nd�r�r
        }

        return -1.0; 
    }

    //sistemin CPU'sundaki s�cakl���n� derece cinsinden d�nd�ren fonksiyon 
    double GetCPUTemperature() {
        std::string line;
        double temperature = -1.0;

        std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp"); // Linux i�in �rnek yolu kullan�yoruz
        if (temperatureFile.is_open()) {
            getline(temperatureFile, line);
            temperature = std::stod(line) / 1000.0; //Dereceye d�nd�r�r
            temperatureFile.close();
        }

        return temperature;
    }

    //Sistemde CPU'nun y�zde ka� kulland���n� d�nd�ren fonksiyon
    double GetCPUUsage() {

        FILETIME idleTime, kernelTime, userTime;

        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            ULONGLONG sysTime = static_cast<ULONGLONG>(kernelTime.dwHighDateTime) << 32 | kernelTime.dwLowDateTime;
            ULONGLONG usrTime = static_cast<ULONGLONG>(userTime.dwHighDateTime) << 32 | userTime.dwLowDateTime;
            ULONGLONG idleTimeVal = static_cast<ULONGLONG>(idleTime.dwHighDateTime) << 32 | idleTime.dwLowDateTime;

            ULONGLONG totalTime = sysTime + usrTime;
            double usagePercent = (1.0 - (static_cast<double>(idleTimeVal) / totalTime)) * 1000.0;

            return usagePercent;
        }
        else {
            return -1.0; 
        }
    
    }

    // Sistemdeki RAM kullan�m�n� y�zde olarak d�nd�ren fonksiyon
    double GetRAMUsage() {
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        return static_cast<double>(statex.dwMemoryLoad); 
    }

    // Sistemde bulunan Toplam Ram boyutunu GB cinsinden d�nd�ren fonksiyon
     int  GetRAMTotal() {

       MEMORYSTATUSEX statex1;
       statex1.dwLength = sizeof(statex1);
       GlobalMemoryStatusEx(&statex1);

       return statex1.ullTotalPhys / (1024 * 1024 * 1024); 
       
    }

     // Sistemde bo�ta olan Ram boyutunu GB cinsinden d�nd�ren fonksiyon
     int  GetRAMEmpty() {

         MEMORYSTATUSEX statex2;
         statex2.dwLength = sizeof(statex2);
         GlobalMemoryStatusEx(&statex2);

         return statex2.ullAvailPhys / (1024 * 1024 * 1024); 
         
     }

 };



    int main() {

        SystemInfo systemInfo;
        
        while (true) {

            //veri aktar�m� i�in kullan�lan txt dosyas�
            std::ofstream outputFile("output.txt");


            //cpu 
            //CPU i�in olu�turulan fonskiyonlar�n kullan�lmas� ve bir de�i�kene atanmas�
            int cpuUsage = systemInfo.GetCPUUsage();


            //ram
            //RAM i�in olu�turulan fonskiyonlar�n kullan�lmas� ve bir de�i�kene atanmas�
            double ramUsage = systemInfo.GetRAMUsage();
            int ramTotal = systemInfo.GetRAMTotal();
            int ramEmpty = systemInfo.GetRAMEmpty();

            //hdd
            //HDD i�in olu�turulan fonskiyonlar�n kullan�lmas� ve bir de�i�kene atanmas�
            int hddFreeSpace = systemInfo.GetHDDFreeSpaceGB(L"C:");
            int hddFullSpace = systemInfo.GetHDDFullSpaceGB(L"C:");


            //sistemden al�nan de�erlerin dosyaya yaz�lma i�lemi 
            if (outputFile.is_open()) {
                
                outputFile << cpuUsage << ",";
                outputFile << ramUsage << ",";
                outputFile << ramTotal << ",";
                outputFile << ramEmpty << ",";
                outputFile << hddFreeSpace << ",";
                outputFile << hddFullSpace << ",";
               

                outputFile.close();

                std::cout << "Variables written to output.txt" << std::endl;
            }
            else {
                std::cerr << "Unable to open the file for writing!" << std::endl;
            }

            Sleep(30000); // 30 saniyede bir veri al

        }
            return 0;

    }
         

         //verilerin pipe kullan�larak aktar�lmas� denendi

          /*Named Pipe olu�turma
            HANDLE pipe = CreateNamedPipe(
                L"\\\\.\\pipe\\MyPipe",          // Pipe ad�
                PIPE_ACCESS_DUPLEX,           // Yaln�zca yazma izni
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                1,                               // Maksimum istemci say�s�
                0, 0, 0, NULL
              

            );
            std::cout << "pipe olustu " << std::endl;

             if (pipe == INVALID_HANDLE_VALUE) {
                            std::cerr << "Named Pipe olu�turma hatas�!" << std::endl;
                            return 1;
                        }

            HANDLE pipe2 = CreateFile(
                L"\\\\.\\pipe\\MyPipe",  // Pipe ad�
                GENERIC_WRITE,           // Yaln�zca yazma izni
                0, NULL,
                OPEN_EXISTING,           // Varolan bir pipe'i a�ma
                0, NULL
            );
 
            if (pipe2 != INVALID_HANDLE_VALUE) {
                // Ba�lant� ba�ar�l�

                // Veri yazma
                std::string data = "Merhaba, Named Pipe!";
                DWORD bytesWritten = 0;
                if (WriteFile(pipe2, data.c_str(), data.size(), &bytesWritten, NULL)) {
                    std::cout << "Veri yazildi: " << bytesWritten << " byte" << std::endl;
                }
                else {
                    std::cerr << "Veri yazma hatasi!" << std::endl;
                }

                // Pipe'i kapatma
                CloseHandle(pipe2);
            }
            else {
                std::cerr << "Ba�lant� hatas�!" << std::endl;
    
            // Named Pipe'e ba�lanma
        if (ConnectNamedPipe(pipe, NULL) != FALSE) {
            std::cout << "deneme0 " << std::endl;
            // Sistem bilgilerini haz�rla
            std::string systemInfo1 = "CPU Kullanimi: ";
            std::cout << "deneme1 " << std::endl;

                // Sistem bilgilerini Named Pipe �zerinden g�nder
            WriteFile(pipe, systemInfo1.c_str(),NULL, NULL, NULL);
            std::cout << "deneme2 " << std::endl;

                // Pipe'i kapatma
            CloseHandle(pipe);
            std::cout << "deneme3 " << std::endl;
            return 0;
            std::cout << "deneme4 " << std::endl;
        }*/  

     
    
