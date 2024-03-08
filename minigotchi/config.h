///////////////////////////////////////////
// config.h: header files for config.cpp //
///////////////////////////////////////////

#ifndef CONFIG_H
#define CONFIG_H

class Config {
public:
    Config();
    static bool deauth;
    static bool advertise; 
    static int whitelist[]; 
    static int channels[];
    
private:

};

#endif // CONFIG_H