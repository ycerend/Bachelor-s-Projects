/**
 * This class contains constant values which represent the constraints of database management system.
 */
public class Constants {

    private Constants(){}
    //constants for system catalogue
    public static final int SYSCATSIZE = 1024*1024; // size of system catalog is 1 MB
    public static final int PAGESIZE = 2048; // in B
    public static final int NUMBEROFTYPES_PAGEHEADER = 4 ;//in bytes    
    public static final int NUMBEROFPAGES_PAGEHEADER = 4 ;//in bytes
    public static final int MAXTYPEIDLENGTH_RECORDHEADER = 4 ;//in bytes    
    public static final int MAXTYPENAMELENGTH_RECORDHEADER = 10;//in bytes
    public static final int MAXNUMBEROFFIELDS_RECORDHEADER =4; //in numbers
    public static final int POINTERTONEXTTYPE_RECORDHEADER =6; //in bytes
    public static final int MAXFIELDLENGTH = 10;
    public static final int MAXFIELDNUMBER = 6; // in B
    public static final int MAXRECORDSIZE = 200; // in B
    public static final int TYPEINFORMATION = 90; // a type info stored in catalog consists of 90 B at total
    //constants for records

    public static final int NUMBEROFFILES = 4; // in B
    public static final int NUMBEROFRECORDS = 4; // in B
    public static final int NUMBEROFPAGES = 4; // in B

    //constants for record files
    public static final int PAGEIDLENGTH = 4;
    public static final int MAXNUMBEROFRECORDS = 10;
    public static final int MAXNUMBEROFPAGES = 10;
    public static final int PAGESIZEFORAFILE = 420 ;
    public static final int PAGEHEADERSIZEFORAFILE = 20 ;
    public static final int LENGTHOFPOINTERFORNEXTPAGE = 20 ;
    public static final int SIZEOFARECORD = 40 ;
    public static final int RECORDID_RECORDHEADER = 4;
    public static final int ISEMPTYINFORMATION_RECORD = 4 ;
    public static final int LENGTHOFONEFIELD = 6 ;


}