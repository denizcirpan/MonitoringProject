#define _WIN32_DCOM
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <wbemidl.h>
#include <comdef.h>


#pragma comment(lib, "wbemuuid.lib")

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
        //LINUX �C�N 
        //std::string line;
        //double temperature = -1.0;

        //std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp"); // Linux i�in �rnek yolu kullan�yoruz
        //if (temperatureFile.is_open()) {
        //    getline(temperatureFile, line);
        //    temperature = std::stod(line) / 1000.0; //Dereceye d�nd�r�r
        //    temperatureFile.close();
        //}

        //return temperature;



        //WMI temperature i�in denendi
        /*HRESULT hres;


        // Initialize COM.
        hres = CoInitializeEx(0, COINIT_MULTITHREADED);

        if (FAILED(hres))
        {
            std::cout << "Failed to initialize COM library. " << std::endl;


            return 1;              // Program has failed.
        }

        // Initialize
        hres = CoInitializeSecurity(
            NULL,
            -1,      // COM negotiates service
            NULL,    // Authentication services
            NULL,    // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
            RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
            NULL,             // Authentication info
            EOAC_NONE,        // Additional capabilities
            NULL              // Reserved
        );


        if (FAILED(hres))
        {
            std::cout << "Failed to initialize security. "

                 <<std:: endl;

            return 1;          // Program has failed.
        }

        // Obtain the initial locator to Windows Management
        // on a particular host computer.
        IWbemLocator* pLoc = 0;

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc);

        if (FAILED(hres))
        {
            std::cout << "Failed to create IWbemLocator object. "

                 << std::endl;

            return 1;       // Program has failed.
        }

        IWbemServices* pSvc = 0;

        // Connect to the root\cimv2 namespace with the
        // current user and obtain pointer pSvc
        // to make IWbemServices calls.

        hres = pLoc->ConnectServer(

            _bstr_t(L"ROOT\\cimv2"), // WMI namespace
            NULL,                    // User name
            NULL,                    // User password
            0,                       // Locale
            NULL,                    // Security flags
            0,                       // Authority
            0,                       // Context object
            &pSvc                    // IWbemServices proxy
        );

        if (FAILED(hres))
        {
            std::cout << "Could not connect. Error code = 0x"
             << std::endl;

            return 1;                // Program has failed.
        }

        std::cout << "Connected to ROOT\\CIMV2 WMI namespace" <<std:: endl;

        // Set the IWbemServices proxy so that impersonation
        // of the user (client) occurs.
        hres = CoSetProxyBlanket(

            pSvc,                         // the proxy to set
            RPC_C_AUTHN_WINNT,            // authentication service
            RPC_C_AUTHZ_NONE,             // authorization service
            NULL,                         // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,       // authentication level
            RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
            NULL,                         // client identity
            EOAC_NONE                     // proxy capabilities
        );

        if (FAILED(hres))
        {
          std::cout << "Could not set proxy blanket. Error code = 0x"
            << std:: endl;
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return 1;               // Program has failed.
        }


        // Use the IWbemServices pointer to make requests of WMI.
        // Make requests here:

        // For example, query for all the running processes
        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvc->ExecQuery(

            bstr_t("SELECT Description FROM Win32_TemperatureProbe"),
            bstr_t("WQL"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

        std::cout << "hres " << hres   << std::endl;



        if (FAILED(hres))
        {
            std::cout << "Query for processes failed. "

                << std::endl;
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return 1;               // Program has failed.
        }
        else
        {
            IWbemClassObject* pclsObj;
            ULONG uReturn = 0;

            while (pEnumerator)
            {
                std::cout << " while girdim " << std::endl;
                hres = pEnumerator->Next(WBEM_INFINITE, 1,
                    &pclsObj, &uReturn);

              /*  if (0 == uReturn)
                {
                    std::cout << " if ciktim " << std::endl;
                    break;
                }

                VARIANT vtProp;

                // Get the value of the Name property
                hres = pclsObj->Get(L"CurrentTemperature", 0, &vtProp, 0, 0);
                std::cout << L" demenmCPU Temperature: " << vtProp.intVal << L" degrees Celsius" << std::endl;
                std::cout << " deneeehres " << hres << std::endl;

                if (SUCCEEDED(hres)) {
                    std::cout << L"CPU Temperature: " << vtProp.intVal << L" degrees Celsius" << std::endl;
                    VariantClear(&vtProp);
                }


                /*hres = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
                std::cout << "Process Name : " << vtProp.bstrVal << std::endl;
                VariantClear(&vtProp);

                pclsObj->Release();
                pclsObj = NULL;
            }

        }

        // Cleanup
        // ========

        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();

        CoUninitialize();

        return 0;  // Program successfully completed.
    }*/

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

     
    
