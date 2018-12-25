//http://msdn.microsoft.com/en-us/library/windows/desktop/ms711730(v=vs.85).aspx
//http://publib.boulder.ibm.com/infocenter/db2luw/v8/index.jsp?topic=/com.ibm.db2.udb.doc/ad/r0000615.htm

//#define DD_DM
//#define WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#include <windows.h>
#include <conio.h>
#endif


#ifdef DD_DM
#include "sql.h"
#include "sqlext.h"
#else
#include <infxcli.h>
#endif
    
typedef struct 
{
    SQLUINTEGER     InfoType;
    char            *InfoName;
}  GetInfoT;

void MyGetInfo( SQLHDBC hdbc );
SQLRETURN  Test1( SQLHDBC hdbc);
void GetDiagRec(SQLRETURN rc, SQLSMALLINT htype, SQLHANDLE hndl, char *szFunTag );
void PrintSupportInfo(SQLUINTEGER InfoValue, GetInfoT GetInfoTArray[], int ArrySize);

void PrintSupport_SQL_TIMEDATE_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_STRING_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_NUMERIC_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_SQL92_NUMERIC_VALUE_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_AGGREGATE_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_SQL92_STRING_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_SQL92_DATETIME_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_CONVERT_FUNCTIONS(SQLUINTEGER InfoValue);
void PrintSupport_SQL_SYSTEM_FUNCTIONS(SQLUINTEGER InfoValue);

int main (long argc, char* argv[])
{

    SQLCHAR ConnStrIn[] = "DRIVER={IBM INFORMIX ODBC DRIVER (64-bit)};SERVER=ids0;DATABASE=ids0db1;HOST=xyz.com;PROTOCOL=onsoctcp;SERVICE=9088;UID=informix;PWD=xyz;";

    SQLHDBC hdbc;
    SQLHENV henv;
    SQLRETURN rc = 0;

    SQLCHAR connStrOut[1024]="";
    SQLSMALLINT connStrOutLen=0;


    if( argc == 2 )
    {
        //sprintf( (char *)ConnStrIn, "DSN=%s", argv[1] );
    }



    rc = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    rc = SQLSetEnvAttr (henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);

    rc = SQLAllocHandle (SQL_HANDLE_DBC, henv, &hdbc);


    //rc = SQLSetConnectAttr(hdbc, SQL_INFX_ATTR_LOCALIZE_DECIMALS, (SQLPOINTER)1,  SQL_IS_INTEGER);

    printf ( "\nConnecting ...");
    printf ( "\n\nConnStrIn = [%s]\n", ConnStrIn);
    rc = SQLDriverConnect ( hdbc, NULL, ConnStrIn, SQL_NTS, connStrOut, 1000, &connStrOutLen,  SQL_DRIVER_NOPROMPT);
    if (rc != SQL_SUCCESS)
    {
        GetDiagRec(rc, SQL_HANDLE_DBC, hdbc, "SQLDriverConnect" );
        SQLFreeHandle (SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle (SQL_HANDLE_ENV, henv);
        return(-1);
    }


    printf ( "Connected!\n");

    MyGetInfo( hdbc );
    //Test1(hdbc);
    
    rc = SQLDisconnect (hdbc);

    rc = SQLFreeHandle (SQL_HANDLE_DBC, hdbc);
    rc = SQLFreeHandle (SQL_HANDLE_ENV, henv);


    printf ( "\n\n End!");

    return (0);
}


void MyGetInfo( SQLHDBC hdbc )
{
    static GetInfoT  GetInfoList[] = 
    { 

/*
          { SQL_CATALOG_NAME_SEPARATOR, "SQL_CATALOG_NAME_SEPARATOR" }, 
          { SQL_CATALOG_TERM, "SQL_CATALOG_TERM" }, 
          { SQL_CATALOG_USAGE, "SQL_CATALOG_USAGE" }, 
          { SQL_COLUMN_ALIAS, "SQL_COLUMN_ALIAS" }, 
          { SQL_DATETIME_LITERALS, "SQL_DATETIME_LITERALS" }, 
          { SQL_DBMS_NAME, "SQL_DBMS_NAME" }, 
          { SQL_DBMS_VER, "SQL_DBMS_VER" }, 
          { SQL_DRIVER_ODBC_VER, "SQL_DRIVER_ODBC_VER" }, 
*/
          { SQL_DRIVER_VER, "SQL_DRIVER_VER" }, 
/*
          { SQL_IDENTIFIER_QUOTE_CHAR, "SQL_IDENTIFIER_QUOTE_CHAR" }, 
          { SQL_MAX_IDENTIFIER_LEN, "SQL_MAX_IDENTIFIER_LEN" }, 
          { SQL_ODBC_INTERFACE_CONFORMANCE, "SQL_ODBC_INTERFACE_CONFORMANCE" }, 
          { SQL_ODBC_VER, "SQL_ODBC_VER" }, 
          { SQL_OJ_CAPABILITIES, "SQL_OJ_CAPABILITIES" }, 
          { SQL_QUOTED_IDENTIFIER_CASE, "SQL_QUOTED_IDENTIFIER_CASE" }, 
          { SQL_SCHEMA_TERM, "SQL_SCHEMA_TERM" }, 
          { SQL_SCHEMA_USAGE, "SQL_SCHEMA_USAGE" }, 
          { SQL_SPECIAL_CHARACTERS, "SQL_SPECIAL_CHARACTERS" }, 
          { SQL_SQL92_PREDICATES, "SQL_SQL92_PREDICATES" }, 
          { SQL_SQL92_RELATIONAL_JOIN_OPERATORS, "SQL_SQL92_RELATIONAL_JOIN_OPERATORS" }, 
          { SQL_SQL92_VALUE_EXPRESSIONS, "SQL_SQL92_VALUE_EXPRESSIONS" }, 
          { SQL_SQL_CONFORMANCE, "SQL_SQL_CONFORMANCE" }, 
          { SQL_TABLE_TERM, "SQL_TABLE_TERM" }, 
*/
          { SQL_SYSTEM_FUNCTIONS, "SQL_SYSTEM_FUNCTIONS" },
          { SQL_CONVERT_FUNCTIONS, "SQL_CONVERT_FUNCTIONS" },
          { SQL_SQL92_DATETIME_FUNCTIONS, "SQL_SQL92_DATETIME_FUNCTIONS" }, 
          { SQL_SQL92_STRING_FUNCTIONS, "SQL_SQL92_STRING_FUNCTIONS" }, 
          { SQL_AGGREGATE_FUNCTIONS, "SQL_AGGREGATE_FUNCTIONS" }, 
          { SQL_SQL92_NUMERIC_VALUE_FUNCTIONS, "SQL_SQL92_NUMERIC_VALUE_FUNCTIONS" }, 
          { SQL_NUMERIC_FUNCTIONS, "SQL_NUMERIC_FUNCTIONS" }, 
          { SQL_STRING_FUNCTIONS, "SQL_STRING_FUNCTIONS" },
          { SQL_TIMEDATE_FUNCTIONS, "SQL_TIMEDATE_FUNCTIONS" },
    };





    SQLRETURN       rc = 0;
    SQLUINTEGER     IntVal=0;

    unsigned char   InfoValue[1024] ="";
    SQLSMALLINT     StringLength=0;
    int             ArrySize=0;
    int             i=0;

    ArrySize = sizeof(GetInfoList) / sizeof(GetInfoT);
    for( i=0; i<ArrySize; ++i)
    {
        SQLUINTEGER            InfoType    = GetInfoList[i].InfoType;
        char                    *InfoName   = GetInfoList[i].InfoName;
        //InfoType = SQL_ODBC_VER;
        //InfoName = "SQL_ODBC_VER";

        StringLength=0;
        memset( InfoValue, 0, sizeof(InfoValue) );
        rc = SQLGetInfo( hdbc, (SQLUSMALLINT)InfoType, InfoValue, sizeof( InfoValue ), &StringLength);
        if ( rc != 0 )
        {
            printf( "\n--- %2d) Error %s=[rc=%d]" , i, InfoName, rc );
            continue;
        }

        printf( "\n\n");

        ++i;
        switch( InfoType )
        {
        case SQL_AGGREGATE_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating support for aggregation functions:
                //SQL_AF_ALL SQL_AF_AVG SQL_AF_COUNT SQL_AF_DISTINCT SQL_AF_MAX SQL_AF_MIN SQL_AF_SUM 
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
                PrintSupport_SQL_AGGREGATE_FUNCTIONS( IntVal );
            } 
            break;

        case SQL_CATALOG_NAME_SEPARATOR : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_CATALOG_TERM : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_CATALOG_USAGE : 
            { 
                //An SQLUINTEGER bitmask enumerating the statements in which catalogs can be used.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_COLUMN_ALIAS : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_CONVERT_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the scalar conversion functions supported by the driver and associated data source.
                //The following bitmask is used to determine which conversion functions are supported:
                //SQL_FN_CVT_CAST SQL_FN_CVT_CONVERT
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
                PrintSupport_SQL_CONVERT_FUNCTIONS(IntVal );
            } 
            break;

        case SQL_DATETIME_LITERALS : 
            { 
                //An SQLUINTEGER bitmask enumerating the SQL-92 datetime literals supported by the data source.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_DBMS_NAME : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_DBMS_VER : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_DRIVER_ODBC_VER : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_DRIVER_VER : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_IDENTIFIER_QUOTE_CHAR : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_MAX_IDENTIFIER_LEN : 
            { 
                //An SQLUSMALLINT that indicates the maximum size in characters that the data source supports for user-defined names. 
                //An FIPS Entry level–conformant driver will return at least 18. An FIPS Intermediate level–conformant driver will return at least 128.
                IntVal = *(SQLUSMALLINT *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_NUMERIC_FUNCTIONS : 
            { 
                 IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );

                PrintSupport_SQL_NUMERIC_FUNCTIONS(IntVal);
            } 
            break;

        case SQL_ODBC_INTERFACE_CONFORMANCE : 
            { 
                //An SQLUINTEGER value that indicates the level of the ODBC 3.x interface that the driver complies with.
                //SQL_OIC_CORE: The minimum level that all ODBC drivers are expected to comply with. This level includes basic interface elements such as connection functions, functions for preparing and executing an SQL statement, basic result set metadata functions, basic catalog functions, and so on.
                //SQL_OIC_LEVEL1: A level including the core standards compliance level functionality, plus scrollable cursors, bookmarks, positioned updates and deletes, and so on.
                //SQL_OIC_LEVEL2: A level including level 1 standards compliance level functionality, plus advanced features such as sensitive cursors; update, delete, and refresh by bookmarks; stored procedure support; catalog functions for primary and foreign keys; multi-catalog support; and so on.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_ODBC_VER : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_OJ_CAPABILITIES : 
            { 
                //An SQLUINTEGER bitmask enumerating the types of outer joins supported by the driver and data source
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_QUOTED_IDENTIFIER_CASE : 
            { 
                //An SQLUSMALLINT value as follows:
                //SQL_IC_UPPER = Quoted identifiers in SQL are not case-sensitive and are stored in uppercase in the system catalog.
                //SQL_IC_LOWER = Quoted identifiers in SQL are not case-sensitive and are stored in lowercase in the system catalog.
                //SQL_IC_SENSITIVE = Quoted identifiers in SQL are case sensitive and are stored in mixed case in the system catalog. 
                //(In an SQL-92–compliant database, quoted identifiers are always case-sensitive.)
                //SQL_IC_MIXED = Quoted identifiers in SQL are not case-sensitive and are stored in mixed case in the system catalog.
                IntVal = *(SQLUSMALLINT *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_SCHEMA_TERM : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_SCHEMA_USAGE : 
            { 
                //An SQLUINTEGER bitmask enumerating the statements in which schemas can be used:
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_SPECIAL_CHARACTERS : 
            { 
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            }
            break;

        case SQL_SQL92_DATETIME_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the datetime scalar functions that are supported by the driver and the associated data source, as defined in SQL-92.
                //The following bitmasks are used to determine which datetime functions are supported:
                //SQL_SDF_CURRENT_DATE SQL_SDF_CURRENT_TIME SQL_SDF_CURRENT_TIMESTAMP

                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
                PrintSupport_SQL_SQL92_DATETIME_FUNCTIONS( IntVal );
            } 
            break;

        case SQL_SQL92_NUMERIC_VALUE_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the numeric value scalar functions that are supported by the driver and the associated data source, as defined in SQL-92.
                //The following bitmasks are used to determine which numeric functions are supported:
                //SQL_SNVF_BIT_LENGTH SQL_SNVF_CHAR_LENGTH SQL_SNVF_CHARACTER_LENGTH SQL_SNVF_EXTRACT SQL_SNVF_OCTET_LENGTH SQL_SNVF_POSITION
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
                PrintSupport_SQL_SQL92_NUMERIC_VALUE_FUNCTIONS( IntVal );
            } 
            break;

        case SQL_SQL92_PREDICATES : 
            { 
                //3
                //An SQLUINTEGER bitmask enumerating the predicates supported in a SELECT statement, as defined in SQL-92. 
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_SQL92_RELATIONAL_JOIN_OPERATORS : 
            { 
                //3
                //An SQLUINTEGER bitmask enumerating the relational join operators supported in a SELECT statement, as defined in SQL-92. 
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_SQL92_STRING_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the string scalar functions that are supported by the driver and the associated data source, as defined in SQL-92.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
                PrintSupport_SQL_SQL92_STRING_FUNCTIONS(IntVal);
            } 
            break;

        case SQL_SQL92_VALUE_EXPRESSIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the value expressions supported, as defined in SQL-92.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_SQL_CONFORMANCE : 
            { 
                //3
                //An SQLUINTEGER value that indicates the level of SQL-92 supported by the driver: 
                //SQL_SC_SQL92_ENTRY = Entry level SQL-92 compliant.
                //SQL_SC_FIPS127_2_TRANSITIONAL = FIPS 127-2 transitional level compliant.
                //SQL_SC_SQL92_FULL = Full level SQL-92 compliant.
                //SQL_SC_ SQL92_INTERMEDIATE = Intermediate level SQL-92 compliant.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_STRING_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the scalar string functions supported by the driver and associated data source.
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );

                PrintSupport_SQL_STRING_FUNCTIONS(IntVal);
            } 
            break;

        case SQL_SYSTEM_FUNCTIONS : 
            { 
                //An SQLUINTEGER bitmask enumerating the scalar system functions supported by the driver and associated data source. 
                //The following bitmasks are used to determine which system functions are supported:
                //SQL_FN_SYS_DBNAMESQL_FN_SYS_IFNULLSQL_FN_SYS_USERNAME
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
                PrintSupport_SQL_SYSTEM_FUNCTIONS( IntVal );
            } 
            break;

        case SQL_TABLE_TERM : 
            { 
                //1
                printf( "\n--- %2d) %s=[%s]" , i, InfoName, InfoValue );
            } 
            break;

        case SQL_TIMEDATE_ADD_INTERVALS : 
            { 
                //2
                //An SQLUINTEGER bitmask enumerating the timestamp intervals supported by the driver and associated data source for the TIMESTAMPADD scalar function.
                //The following bitmasks are used to determine which intervals are supported:
                //SQL_FN_TSI_FRAC_SECOND SQL_FN_TSI_SECOND SQL_FN_TSI_MINUTE SQL_FN_TSI_HOUR SQL_FN_TSI_DAY SQL_FN_TSI_WEEK SQL_FN_TSI_MONTH SQL_FN_TSI_QUARTER SQL_FN_TSI_YEAR
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_TIMEDATE_DIFF_INTERVALS : 
            { 
                //2
                //An SQLUINTEGER bitmask enumerating the timestamp intervals supported by the driver and associated data source for the TIMESTAMPDIFF scalar function.
                //The following bitmasks are used to determine which intervals are supported:
                //SQL_FN_TSI_FRAC_SECOND SQL_FN_TSI_SECOND SQL_FN_TSI_MINUTE SQL_FN_TSI_HOUR SQL_FN_TSI_DAY SQL_FN_TSI_WEEK SQL_FN_TSI_MONTH SQL_FN_TSI_QUARTER SQL_FN_TSI_YEAR 
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );
            } 
            break;

        case SQL_TIMEDATE_FUNCTIONS : 
            { 
                //1
                //An SQLUINTEGER bitmask enumerating the scalar date and time functions supported by the driver and associated data source.
                //The following bitmasks are used to determine which date and time functions are supported:
                //SQL_FN_TD_CURRENT_DATE ODBC 3.0)SQL_FN_TD_CURRENT_TIME (ODBC 3.0)SQL_FN_TD_CURRENT_TIMESTAMP (ODBC 3.0)
                //SQL_FN_TD_CURDATE (ODBC 1.0)SQL_FN_TD_CURTIME (ODBC 1.0) SQL_FN_TD_DAYNAME (ODBC 2.0)SQL_FN_TD_DAYOFMONTH (ODBC 1.0)
                //SQL_FN_TD_DAYOFWEEK (ODBC 1.0)SQL_FN_TD_DAYOFYEAR (ODBC 1.0) SQL_FN_TD_EXTRACT (ODBC 3.0)SQL_FN_TD_HOUR (ODBC 1.0)
                //SQL_FN_TD_MINUTE (ODBC 1.0)SQL_FN_TD_MONTH (ODBC 1.0)SQL_FN_TD_MONTHNAME (ODBC 2.0)SQL_FN_TD_NOW (ODBC 1.0)
                //SQL_FN_TD_QUARTER (ODBC 1.0)SQL_FN_TD_SECOND (ODBC 1.0)SQL_FN_TD_TIMESTAMPADD (ODBC 2.0)SQL_FN_TD_TIMESTAMPDIFF (ODBC 2.0)
                //SQL_FN_TD_WEEK (ODBC 1.0)SQL_FN_TD_YEAR (ODBC 1.0)
                IntVal = *(SQLUINTEGER *)InfoValue;
                printf( "\n--- %2d) %s=[%u]" , i, InfoName, IntVal );

                PrintSupport_SQL_TIMEDATE_FUNCTIONS(IntVal);

            } 
            break;

       default :
            printf( "\n--- %2d) %s=[UnKnown]" , i, InfoName  );
            break;
        }
        --i;
    }

    
    //rc = SQLGetInfo( hdbc, InfoType, InfoValue, sizeof( InfoValue ), &StringLength);
}



SQLRETURN  Test1(SQLHDBC hdbc)
{
    SQLRETURN   rc = 0;
    SQLRETURN   ErrCount = 0;
    SQLHSTMT    hstmt = NULL;
    SQLCHAR     Column_Value[1024]="";
    SQLLEN      LenOrIndPtr;
    int         RecNum=0;
    int         i=0;

    char        *SetupSqls []=
    {
        "DROP TABLE mytab1", 
        "DROP TABLE mytab2", 

        "CREATE TABLE mytab1 ( C1 INT, C2 INT )",
        "CREATE TABLE mytab2 ( C1 INT, C2 INT )",

        "INSERT INTO mytab1  ( C1, C2 ) VALUES ( 1, null)", 
        "INSERT INTO mytab1  ( C1, C2 ) VALUES ( 2, 22 )", 
        NULL
    };

    char *Sql = "SELECT C2 FROM mytab1 ORDER BY C1";


    ///////////////////////// STMT //////////////////////

    rc = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &hstmt );

    //////// Server side setup SQLs /////// 
    for ( i=0; SetupSqls[i] != NULL; ++i)
    {
        rc = SQLExecDirect( hstmt, (SQLCHAR *)SetupSqls[i], SQL_NTS);
    }

    rc = SQLBindCol(hstmt, 1, SQL_C_CHAR,  Column_Value, sizeof(Column_Value), &LenOrIndPtr);

    rc = SQLExecDirect( hstmt,  (SQLCHAR *)Sql, SQL_NTS);

    while (rc==0 && TRUE)
    {
        memset( Column_Value, 0, sizeof(Column_Value) );
        memset( &LenOrIndPtr, 0, sizeof(LenOrIndPtr) );

        if ((rc = SQLFetch(hstmt)) == SQL_NO_DATA)
        {
            break;
        }
        ++RecNum;

        if ( LenOrIndPtr == SQL_NULL_DATA)
        {
           printf ("\n %d [SQL_NULL_DATA]", RecNum);
        }
        else
        {
           printf ("\n %d [%s]", RecNum, Column_Value);
        }

        rc = SQLExecDirect( hstmt, (SQLCHAR *)"INSERT INTO mytab2  ( C1, C2 ) VALUES ( 3, 303 )", SQL_NTS);
        GetDiagRec(rc, SQL_HANDLE_STMT, hstmt, "SQLExecDirect");

        

    }

    //rc = SQLFreeStmt ( hstmt, SQL_CLOSE );
    rc = SQLCloseCursor( hstmt );

    rc = SQLFreeHandle ( SQL_HANDLE_STMT, hstmt );

    rc = 0;
    if( RecNum==0 || ErrCount != 0)
    {
        rc = -1;
    }

    printf ("\n End \n");

    return (rc);
}



void GetDiagRec(SQLRETURN rc, SQLSMALLINT htype, SQLHANDLE hndl, char *szFunTag )
{ 
    SQLCHAR message[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER sqlcode;
    SQLSMALLINT length;

    if ( szFunTag == NULL )
    {
        szFunTag = "---";
    }

    printf  ( "\n %s: %d : ", szFunTag, rc);

    if (rc >= 0)
    {
       printf  ( " OK [rc=%d]", rc);
    }
    else
    {  
        int i=1;

        printf  ( " FAILED: %i", rc);
       /* get multiple field settings of diagnostic record */
       while (SQLGetDiagRec(htype,
                            hndl,
                            i,
                            sqlstate,
                            &sqlcode,
                            message,
                            SQL_MAX_MESSAGE_LENGTH + 1,
                            &length) == SQL_SUCCESS)
       {
         printf  ( "\n SQLSTATE          = %s", sqlstate);

         printf( "\n Native Error Code = %ld", sqlcode);

         printf  ( "\n %s", message);
         i++;
       }
       printf  ( "\n-------------------------\n");
    }
}



void PrintSupportInfo(SQLUINTEGER InfoValue, GetInfoT GetInfoTArray[], int ArrySize)
{
    int i=0;

    printf( "\n{\n");
    for( i=0; i<ArrySize; ++i)
    {
        SQLUINTEGER             InfoType    = GetInfoTArray[i].InfoType;
        char                    *InfoName   = GetInfoTArray[i].InfoName;

        int st = InfoValue & InfoType;

        printf( "\n      %2d) [%s] %s", i+1, st?"YES":"NO ", InfoName);
    }
    printf( "\n}\n");
}


void PrintSupport_SQL_TIMEDATE_FUNCTIONS(SQLUINTEGER InfoValue)
{
/*
   1) SQL_TIMEDATE_FUNCTIONS=[1016623]

  1) [YES] SQL_FN_TD_NOW
  2) [YES] SQL_FN_TD_CURDATE
  3) [YES] SQL_FN_TD_DAYOFMONTH
  4) [YES] SQL_FN_TD_DAYOFWEEK
  5) [NO ] SQL_FN_TD_DAYOFYEAR
  6) [YES] SQL_FN_TD_MONTH
  7) [NO ] SQL_FN_TD_QUARTER
  8) [NO ] SQL_FN_TD_WEEK
  9) [YES] SQL_FN_TD_YEAR
 10) [YES] SQL_FN_TD_CURTIME
 11) [NO ] SQL_FN_TD_HOUR
 12) [NO ] SQL_FN_TD_MINUTE
 13) [NO ] SQL_FN_TD_SECOND
 14) [NO ] SQL_FN_TD_TIMESTAMPADD
 15) [NO ] SQL_FN_TD_TIMESTAMPDIFF
 16) [YES] SQL_FN_TD_DAYNAME
 17) [YES] SQL_FN_TD_MONTHNAME
 18) [YES] SQL_FN_TD_CURRENT_DATE
 19) [YES] SQL_FN_TD_CURRENT_TIME
 20) [YES] SQL_FN_TD_CURRENT_TIMESTAMP
 21) [NO ] SQL_FN_TD_EXTRACT
*/

    static GetInfoT  GetTimeDateFuncList[] = 
    { 
        { SQL_FN_TD_NOW, "SQL_FN_TD_NOW" },

        { SQL_FN_TD_CURDATE, "SQL_FN_TD_CURDATE" },
        { SQL_FN_TD_DAYOFMONTH, "SQL_FN_TD_DAYOFMONTH" },
        { SQL_FN_TD_DAYOFWEEK, "SQL_FN_TD_DAYOFWEEK" },
        { SQL_FN_TD_DAYOFYEAR, "SQL_FN_TD_DAYOFYEAR" },
        { SQL_FN_TD_MONTH, "SQL_FN_TD_MONTH" },
        { SQL_FN_TD_QUARTER, "SQL_FN_TD_QUARTER" },
        { SQL_FN_TD_WEEK, "SQL_FN_TD_WEEK" },
        { SQL_FN_TD_YEAR, "SQL_FN_TD_YEAR" },
        { SQL_FN_TD_CURTIME, "SQL_FN_TD_CURTIME" },
        { SQL_FN_TD_HOUR, "SQL_FN_TD_HOUR" },
        { SQL_FN_TD_MINUTE, "SQL_FN_TD_MINUTE" },
        { SQL_FN_TD_SECOND, "SQL_FN_TD_SECOND" },
        { SQL_FN_TD_TIMESTAMPADD, "SQL_FN_TD_TIMESTAMPADD" },
        { SQL_FN_TD_TIMESTAMPDIFF, "SQL_FN_TD_TIMESTAMPDIFF" },
        { SQL_FN_TD_DAYNAME, "SQL_FN_TD_DAYNAME" },
        { SQL_FN_TD_MONTHNAME, "SQL_FN_TD_MONTHNAME" },

        // ODBC 3.0 App Only
        { SQL_FN_TD_CURRENT_DATE, "SQL_FN_TD_CURRENT_DATE" },
        { SQL_FN_TD_CURRENT_TIME, "SQL_FN_TD_CURRENT_TIME" },
        { SQL_FN_TD_CURRENT_TIMESTAMP, "SQL_FN_TD_CURRENT_TIMESTAMP" },
        { SQL_FN_TD_EXTRACT, "SQL_FN_TD_EXTRACT" },
    };

    int             ArrySize=0;

    ArrySize = sizeof(GetTimeDateFuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, GetTimeDateFuncList, ArrySize );
}




void PrintSupport_SQL_STRING_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
        { SQL_FN_STR_CONCAT , "SQL_FN_STR_CONCAT" },
        { SQL_FN_STR_INSERT , "SQL_FN_STR_INSERT" },
        { SQL_FN_STR_LEFT , "SQL_FN_STR_LEFT" },
        { SQL_FN_STR_LTRIM , "SQL_FN_STR_LTRIM" },
        { SQL_FN_STR_LENGTH , "SQL_FN_STR_LENGTH" },
        { SQL_FN_STR_LOCATE , "SQL_FN_STR_LOCATE" },
        { SQL_FN_STR_LCASE , "SQL_FN_STR_LCASE" },
        { SQL_FN_STR_REPEAT , "SQL_FN_STR_REPEAT" },
        { SQL_FN_STR_REPLACE , "SQL_FN_STR_REPLACE" },
        { SQL_FN_STR_RIGHT , "SQL_FN_STR_RIGHT" },
        { SQL_FN_STR_RTRIM , "SQL_FN_STR_RTRIM" },
        { SQL_FN_STR_SUBSTRING , "SQL_FN_STR_SUBSTRING" },
        { SQL_FN_STR_UCASE , "SQL_FN_STR_UCASE" },
        { SQL_FN_STR_ASCII , "SQL_FN_STR_ASCII" },
        { SQL_FN_STR_CHAR , "SQL_FN_STR_CHAR" },
        { SQL_FN_STR_DIFFERENCE , "SQL_FN_STR_DIFFERENCE" },
        { SQL_FN_STR_LOCATE_2 , "SQL_FN_STR_LOCATE_2" },
        { SQL_FN_STR_SOUNDEX , "SQL_FN_STR_SOUNDEX" },
        { SQL_FN_STR_SPACE , "SQL_FN_STR_SPACE" },

        // ODBCVER >= 0x0300
        { SQL_FN_STR_BIT_LENGTH , "SQL_FN_STR_BIT_LENGTH" },
        { SQL_FN_STR_CHAR_LENGTH , "SQL_FN_STR_CHAR_LENGTH" },
        { SQL_FN_STR_CHARACTER_LENGTH , "SQL_FN_STR_CHARACTER_LENGTH" },
        { SQL_FN_STR_OCTET_LENGTH , "SQL_FN_STR_OCTET_LENGTH" },
        { SQL_FN_STR_POSITION , "SQL_FN_STR_POSITION" },
    };

    int             ArrySize=0;

    ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}




void PrintSupport_SQL_NUMERIC_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
        { SQL_FN_NUM_ABS , "SQL_FN_NUM_ABS" },
        { SQL_FN_NUM_ACOS , "SQL_FN_NUM_ACOS" },
        { SQL_FN_NUM_ASIN , "SQL_FN_NUM_ASIN" },
        { SQL_FN_NUM_ATAN , "SQL_FN_NUM_ATAN" },
        { SQL_FN_NUM_ATAN2 , "SQL_FN_NUM_ATAN2" },
        { SQL_FN_NUM_CEILING , "SQL_FN_NUM_CEILING" },
        { SQL_FN_NUM_COS , "SQL_FN_NUM_COS" },
        { SQL_FN_NUM_COT , "SQL_FN_NUM_COT" },
        { SQL_FN_NUM_EXP , "SQL_FN_NUM_EXP" },
        { SQL_FN_NUM_FLOOR , "SQL_FN_NUM_FLOOR" },
        { SQL_FN_NUM_LOG , "SQL_FN_NUM_LOG" },
        { SQL_FN_NUM_MOD , "SQL_FN_NUM_MOD" },
        { SQL_FN_NUM_SIGN , "SQL_FN_NUM_SIGN" },
        { SQL_FN_NUM_SIN , "SQL_FN_NUM_SIN" },
        { SQL_FN_NUM_SQRT , "SQL_FN_NUM_SQRT" },
        { SQL_FN_NUM_TAN , "SQL_FN_NUM_TAN" },
        { SQL_FN_NUM_PI , "SQL_FN_NUM_PI" },
        { SQL_FN_NUM_RAND , "SQL_FN_NUM_RAND" },
        { SQL_FN_NUM_DEGREES , "SQL_FN_NUM_DEGREES" },
        { SQL_FN_NUM_LOG10 , "SQL_FN_NUM_LOG10" },
        { SQL_FN_NUM_POWER , "SQL_FN_NUM_POWER" },
        { SQL_FN_NUM_RADIANS , "SQL_FN_NUM_RADIANS" },
        { SQL_FN_NUM_ROUND , "SQL_FN_NUM_ROUND" },
        { SQL_FN_NUM_TRUNCATE , "SQL_FN_NUM_TRUNCATE" },
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}




void PrintSupport_SQL_SQL92_NUMERIC_VALUE_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
        { SQL_SNVF_BIT_LENGTH,  "SQL_SNVF_BIT_LENGTH" },
        { SQL_SNVF_CHAR_LENGTH,  "SQL_SNVF_CHAR_LENGTH" },
        { SQL_SNVF_CHARACTER_LENGTH,  "SQL_SNVF_CHARACTER_LENGTH" },
        { SQL_SNVF_EXTRACT,  "SQL_SNVF_EXTRACT" },
        { SQL_SNVF_OCTET_LENGTH,  "SQL_SNVF_OCTET_LENGTH" },
        { SQL_SNVF_POSITION,  "SQL_SNVF_POSITION" },
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}


void PrintSupport_SQL_AGGREGATE_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
        { SQL_AF_AVG,  "SQL_AF_AVG" }, 
        { SQL_AF_COUNT,  "SQL_AF_COUNT" }, 
        { SQL_AF_MAX,  "SQL_AF_MAX" }, 
        { SQL_AF_MIN,  "SQL_AF_MIN" }, 
        { SQL_AF_SUM,  "SQL_AF_SUM" }, 
        { SQL_AF_DISTINCT,  "SQL_AF_DISTINCT" }, 
        { SQL_AF_ALL,  "SQL_AF_ALL" }, 
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}




void PrintSupport_SQL_SQL92_STRING_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
        { SQL_SSF_CONVERT,  "SQL_SSF_CONVERT" }, 
        { SQL_SSF_LOWER,  "SQL_SSF_LOWER" }, 
        { SQL_SSF_UPPER,  "SQL_SSF_UPPER" }, 
        { SQL_SSF_SUBSTRING,  "SQL_SSF_SUBSTRING" }, 
        { SQL_SSF_TRANSLATE,  "SQL_SSF_TRANSLATE" }, 
        { SQL_SSF_TRIM_BOTH,  "SQL_SSF_TRIM_BOTH" }, 
        { SQL_SSF_TRIM_LEADING,  "SQL_SSF_TRIM_LEADING" }, 
        { SQL_SSF_TRIM_TRAILING,  "SQL_SSF_TRIM_TRAILING" }, 
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}


void PrintSupport_SQL_SQL92_DATETIME_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
         { SQL_SDF_CURRENT_DATE,  "SQL_SDF_CURRENT_DATE" }, 
         { SQL_SDF_CURRENT_TIME,  "SQL_SDF_CURRENT_TIME" }, 
         { SQL_SDF_CURRENT_TIMESTAMP,  "SQL_SDF_CURRENT_TIMESTAMP" }, 
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}

void PrintSupport_SQL_CONVERT_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
         { SQL_FN_CVT_CONVERT,  "SQL_FN_CVT_CONVERT" }, 
         { SQL_FN_CVT_CAST,  "SQL_FN_CVT_CAST" }, 
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}


void PrintSupport_SQL_SYSTEM_FUNCTIONS(SQLUINTEGER InfoValue)
{
    static GetInfoT  FuncList[] = 
    { 
         { SQL_FN_SYS_USERNAME,  "SQL_FN_SYS_USERNAME" }, 
         { SQL_FN_SYS_DBNAME,  "SQL_FN_SYS_DBNAME" }, 
         { SQL_FN_SYS_IFNULL,  "SQL_FN_SYS_IFNULL" }, 
    };

    int ArrySize = sizeof(FuncList) / sizeof(GetInfoT);

    PrintSupportInfo( InfoValue, FuncList, ArrySize );
}