//---------------------------------------------------------------------------

#ifndef AppIniSettingsH
#define AppIniSettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

// настройки из ini-файла
class TAppIniSettings
{
private:
    String FHostOracle;
    String FPortOracle;
    String FBaseOracle;
    String FSchemaDatabase;
    String FUsernameOracle;
    String FPasswordOracle;
public:
    __property String HostOracle = { read = FHostOracle, write = FHostOracle };
    __property String PortOracle = { read = FPortOracle, write = FPortOracle };
    __property String BaseOracle = { read = FBaseOracle, write = FBaseOracle };
    __property String SchemaDatabase = { read = FSchemaDatabase, write = FSchemaDatabase };
    __property String UsernameOracle = { read = FUsernameOracle, write = FUsernameOracle };
    __property String PasswordOracle = { read = FPasswordOracle, write = FPasswordOracle };
};

#endif
