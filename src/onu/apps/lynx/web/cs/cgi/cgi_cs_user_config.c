#include <stdio.h>
#include "cgi_cmn.h"
#include "cgi_cs_user_config.h"
#include "db_instance.h"

static cgi_cs_account_type_e g_current_account; 

cgi_status cgi_cs_admin_account_set(
        cs_uint8 *usr,
        cs_uint8 *pwd)
{
    db_ctc_user_t admin;

    CGI_ASSERT_RET((NULL != usr 
                             &&(NULL != pwd)), CGI_ERROR_PARAM);

    memset(&admin,0x00,sizeof(admin));
    strncpy(admin.usr, usr, DB_CTC_MAX_USR_LEN);
    strncpy(admin.pwd, pwd, DB_CTC_MAX_PWD_LEN);
    db_write(DB_CTC_ADMIN_INFO_ID,(cs_uint8*) &admin,sizeof(admin));

    return CGI_OK;
}

cgi_status cgi_cs_user_account_set(
        cs_uint8 *usr,
        cs_uint8 *pwd)
{
    db_ctc_user_t user; 

    CGI_ASSERT_RET((NULL != usr 
                             &&(NULL != pwd)), CGI_ERROR_PARAM);
    memset(&user,0x00,sizeof(user));
    strncpy(user.usr, usr, DB_CTC_MAX_USR_LEN);
    strncpy(user.pwd, pwd, DB_CTC_MAX_PWD_LEN);
    db_write(DB_CTC_USER_INFO_ID,(cs_uint8*) &user,sizeof(user));

    return CGI_OK;
}

cgi_status cgi_cs_admin_account_get(
        cs_uint8 *usr,
        cs_uint8 *pwd)
{
    db_ctc_user_t admin;

    CGI_ASSERT_RET((NULL != usr 
                             &&(NULL != pwd)), CGI_ERROR_PARAM);
    memset(&admin,0x00,sizeof(admin));
    db_read(DB_CTC_ADMIN_INFO_ID,(cs_uint8*) &admin,sizeof(admin));

    strncpy(usr, admin.usr, DB_CTC_MAX_USR_LEN);
    strncpy(pwd, admin.pwd, DB_CTC_MAX_PWD_LEN);

    return CGI_OK;
}

cgi_status cgi_cs_user_account_get(
        cs_uint8 *usr,
        cs_uint8 *pwd)
{
    db_ctc_user_t user; 

    CGI_ASSERT_RET((NULL != usr 
                             &&(NULL != pwd)), CGI_ERROR_PARAM);
    memset(&user,0x00,sizeof(user));
    db_read(DB_CTC_USER_INFO_ID,(cs_uint8*) &user,sizeof(user));
    strncpy(usr, user.usr, DB_CTC_MAX_USR_LEN);
    strncpy(pwd, user.pwd, DB_CTC_MAX_PWD_LEN);

    return CGI_OK;
}


void cgi_cs_get_cookie_str(char *cokieStr)
{
    cs_uint8 usr[DB_CTC_MAX_USR_LEN + 1]; 
    db_ctc_user_t admin;
    db_ctc_user_t user; 

    memset(&admin,0x00,sizeof(admin));
    db_read(DB_CTC_ADMIN_INFO_ID,(cs_uint8*) &admin,sizeof(admin));
    memset(&user,0x00,sizeof(user));
    db_read(DB_CTC_USER_INFO_ID,(cs_uint8*) &user,sizeof(user));


    if(g_current_account == CGI_CTC_ADMIN)
    {
        strncpy(usr, admin.usr, DB_CTC_MAX_USR_LEN);
    }
    else
    {
        strncpy(usr, user.usr, DB_CTC_MAX_USR_LEN);
    }

    sprintf(cokieStr, "user=%s|%s|%s|none|0;", usr,admin.usr, user.usr);
}

cs_boolean cgi_cs_do_authorization(char *puser, char *passwd)
{
    cs_boolean is_authorized = FALSE;

    db_ctc_user_t admin;
    db_ctc_user_t user; 

    memset(&admin,0x00,sizeof(admin));
    db_read(DB_CTC_ADMIN_INFO_ID,(cs_uint8*) &admin,sizeof(admin));
    memset(&user,0x00,sizeof(user));
    db_read(DB_CTC_USER_INFO_ID,(cs_uint8*) &user,sizeof(user));

    if ((strcmp(user.usr,puser) == 0)
            && (strcmp(user.pwd, passwd) == 0))
    {
        is_authorized = TRUE;
        g_current_account = CGI_CTC_USER;
    }

    if ((strcmp(admin.usr,puser) == 0)
            && (strcmp(admin.pwd, passwd) == 0))
    {
        is_authorized = TRUE;
        g_current_account = CGI_CTC_ADMIN;
    }
    
    return is_authorized;
}


void cgi_cs_auth_init()
{
    cyg_auth_handlers_reg(cgi_cs_do_authorization,
            cgi_cs_get_cookie_str,
            NULL);
}


void cgi_cs_account_init()
{
    cgi_cs_auth_init();
}

cgi_status cgi_cs_admin_password_set(cgi_handler_param_t *p)
{
    char name[DB_CTC_MAX_USR_LEN + 1];
    char pwd_old[DB_CTC_MAX_PWD_LEN + 1];
    char pwd_new[DB_CTC_MAX_PWD_LEN + 1];
    char usr[DB_CTC_MAX_USR_LEN + 1];
    char pwd[DB_CTC_MAX_PWD_LEN + 1];

    memset(name, 0, sizeof(name));
    memset(pwd_old, 0, sizeof(pwd_old));
    memset(pwd_new, 0, sizeof(pwd_new));
    memset(usr, 0, sizeof(usr));
    memset(pwd, 0, sizeof(pwd));

    /*get admin name and password*/
    cgi_cs_admin_account_get(usr, pwd);

    if(cgi_get_string_by_key(p, "name", name, DB_CTC_MAX_USR_LEN)) 
    {
        CGI_LOG("can not get name\n");
        cgi_send_int(p, CGI_ERROR_PARAM);
        return CGI_ERROR_PARAM;
    }

    if(cgi_get_string_by_key(p, "pwdold", pwd_old, DB_CTC_MAX_PWD_LEN))
    {
        CGI_LOG("can not get old pwd\n");
        cgi_send_int(p, CGI_ERROR_PARAM);
        return CGI_ERROR_PARAM;
    }

    if(cgi_get_string_by_key(p, "pwdnew", pwd_new, DB_CTC_MAX_PWD_LEN)) 
    {
        CGI_LOG("can not get new pwd\n");
        cgi_send_int(p, CGI_ERROR_PARAM);
        return CGI_ERROR_PARAM;
    }

    /*compare the old password*/
    if(strcmp(pwd_old, pwd))
    {
        cgi_send_int(p, CGI_ERROR_PARAM);
        CGI_DEBUG("old_pwd=%s,pwd=%s", pwd_old, pwd);
        return CGI_ERROR_PARAM;
    }

    /*set the new password*/
    cgi_cs_admin_account_set(name, pwd_new);

    cgi_send_int(p, CGI_OK);
    return CGI_OK;
}


cgi_status cgi_cs_user_password_set(cgi_handler_param_t *p)
{
    char name[DB_CTC_MAX_USR_LEN + 1];
    char pwd_old[DB_CTC_MAX_PWD_LEN + 1];
    char pwd_new[DB_CTC_MAX_PWD_LEN + 1];
    char usr[DB_CTC_MAX_USR_LEN + 1];
    char pwd[DB_CTC_MAX_PWD_LEN + 1];

    memset(name, 0, sizeof(name));
    memset(pwd_old, 0, sizeof(pwd_old));
    memset(pwd_new, 0, sizeof(pwd_new));
    memset(usr, 0, sizeof(usr));
    memset(pwd, 0, sizeof(pwd));

    /*get admin name and password*/
    cgi_cs_user_account_get(usr, pwd);

    if(cgi_get_string_by_key(p, "name", name, DB_CTC_MAX_USR_LEN)) 
    {
        cgi_send_int(p, CGI_ERROR_PARAM);
        return CGI_ERROR_PARAM;
    }

    if(cgi_get_string_by_key(p, "pwdold", pwd_old, DB_CTC_MAX_PWD_LEN))
    {
        cgi_send_int(p, CGI_ERROR_PARAM);
        return CGI_ERROR_PARAM;
    }

    if(cgi_get_string_by_key(p, "pwdnew", pwd_new, DB_CTC_MAX_PWD_LEN)) 
    {
        cgi_send_int(p, CGI_ERROR_PARAM);
        return CGI_ERROR_PARAM;
    }

    /*compare the old password*/
    if(strcmp(pwd_old, pwd))
    {
        cgi_send_int(p, CGI_ERROR_PARAM);
        CGI_DEBUG("old_pwd=%s,pwd=%s", pwd_old, pwd);
        return CGI_ERROR_PARAM;
    }

    /*set the new password*/
    cgi_cs_user_account_set(name, pwd_new);

    cgi_send_int(p, CGI_OK);
    return CGI_OK;
}
