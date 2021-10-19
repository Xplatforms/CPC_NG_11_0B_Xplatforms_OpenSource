// Der folgende ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem CPCNG110BXPLATFORMSOPENSOURCE_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf für kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, sehen
// CPCNG110BXPLATFORMSOPENSOURCE_API-Funktionen als aus einer DLL importiert an, während diese DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef CPCNG110BXPLATFORMSOPENSOURCE_EXPORTS
#define CPCNG110BXPLATFORMSOPENSOURCE_API __declspec(dllexport)
#else
#define CPCNG110BXPLATFORMSOPENSOURCE_API __declspec(dllimport)
#endif



CPCNG110BXPLATFORMSOPENSOURCE_API const char * __cdecl GetECUName(void);
CPCNG110BXPLATFORMSOPENSOURCE_API const char * __cdecl GetComment(void);
CPCNG110BXPLATFORMSOPENSOURCE_API int __cdecl GetConfiguredAccessTypes(int *);
CPCNG110BXPLATFORMSOPENSOURCE_API int __cdecl GetSeedLength(int);
CPCNG110BXPLATFORMSOPENSOURCE_API int __cdecl GetKeyLength(int);
CPCNG110BXPLATFORMSOPENSOURCE_API int __cdecl GenerateKeyExOpt(const unsigned char* ipSeedArray,
unsigned int iSeedArraySize,
const unsigned int iSecurityLevel,
const char * ipVariant, //[in] ipVariant: the ECU variant’s qualifier
const char* ipOptions, //[in] ipOptions: the option string(free text)
unsigned char* iopKeyArray,
unsigned int iMaxKeyArraySize,
unsigned int& oActualKeyArraySize);


