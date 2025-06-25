#include <stdio.h>
#include <string.h> 

#include <stdlib.h>
#include <stdbool.h>

#include "remote_loader.h"

// mo phong tai file plugin tu xa // 
RemoteLoadStatus download_and_verify_plugin(const RemotePluginMeta *remote){
    if (!remote)
    {
        log_remote_plugin_event (remote, LOAD_FAILURE); 
        return LOAD_FAILURE; 
    }
    printf("REmote loader - Bat dau tai plugin tu :", remote -> source_ur);

    // buoc 1 : Xac minh chu ky .. // 
    if (!is_plugin_signed_correctly(remote))
    {
        printf("REmote loader - Sai chu ky so"); 
        log_remote_plugin_event (remote, LOAD_INVALID_SIGNATURE); 
        return LOAD_INVALID_SIGNATURE; 
    }

    // buoc 2 : Xac minh check sum .. //
    if (!validate_plugin_checksum(remote -> plug_in_name, remote ->checksum))
    {
        printf("Remote Loader - Sai ma bam check sum"); 
        log_remote_plugin_event(remote, LOAD_FAILURE);
        return LOAD_FAILURE; 
    }

    // Buoc 3 : Cai dat plugin .. // 
    if (!install_remote_plugin(remote -> plug_in_name))
    {
        printf("Remote Loader - Cai dat that bai"); 
        log_remote_plugin_event (remote, LOAD_FAILURE); 
        return LOAD_FAILURE; 
    }

    printf("Remote loader  - plugin '%s' da duoc cai dat san thanh cong ", remote -> plug_in_name);
    log_remote_plugin_event(remote, LOAD_SUCCESS);
    return LOAD_SUCCESS;  
}

// Gia lap kiem tra chu ky so .. // 
bool is_plugin_signed_correctly(const RemotePluginMeta  *remote)
{
    // Todo : Su dung xac minh chu ky RSA hoac GPG Thuc .. // 
    if (strcmp(remote -> signature, "valid_signatrue") == 0)
    {
        return true; 
    }
    return false; 
}

// Gia lap kiem tra checksum .. // 
bool validate_plugin_checksum(const char *filepath, const char *expected_checksum )
{
    // Todo : tinh toan thuc te SHA 256 check sum cua file .. 
    if (strcmp(expected_checksum, "Abc123") == 0)
    {
        return true;
    }
    return false; 
}

// Gia lap cai dat plugin .. // 
bool install_remote_plugin(const char *filepath)
{
    // TOdo : Thuc hien dlopen(), sao chep plugin, phan quyen ..// 
    printf("Remote loader -> Cat dat plugin tu tep : %s ...", filepath); 
    return true; 
}

// Ghi log qua trinh xu ly .. // 
void log_remote_plugin_event(const RemotePluginMeta *meta, RemoteLoadStatus status)
{
    const char *status_str;  
    switch(status)
    {
        case LOAD_SUCCESS : status_str = "SUCCESS"; 
        break;
        case LOAD_FAILURE : status_str = "Failure"; 
        break; 
        case LOAD_TRUST_DENIED : status_str = "Trust_Denied"; 
        break;
        case LOAD_TIMEOUT : status_str = "TIME OUT" ; 
        break; 
        case LOAD_INVALID_SIGNATURE  : status_str = "INVALID";
        break; 
        default : status_str = "Unknown"; 
        break; 
    }

    printf
    (
        "Remote Loader - LOG Plugin : %s | Source : %s |Source : %s\n"
        , meta  ? meta ->plug_in_name : "null", 
        meta ? meta -> source_ur : "null"
        , 
        status_str
    ) ;

}
