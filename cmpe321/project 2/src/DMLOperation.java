import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.nio.file.*;
/**
 * This class contains constant values which represent the constraints of database management system.
 */
public class DMLOperation {

    public static void sortRecords(String []s, int numberOfRecords) 
    { 

            //int temp = Integer.parseInt(DDLOperation.actualData( s[i])) ;    
            String swap;
            for(int i=0;i<s.length-1;i++){
                for(int j=0;j<s.length-1-i;j++){
                    int temp1 = Integer.parseInt(DDLOperation.actualData( s[j])) ;
                    int temp2 = Integer.parseInt(DDLOperation.actualData( s[j+1])) ;
                    //if(a[j]>a[j+1]){
                    if(temp1>temp2){
                        swap=s[j];
                        s[j]=s[j+1];
                        s[j+1]=swap;
                    }
                } 
            }
            
              
    } 
    public static String record_byteToString(byte[] record){
        String record_str ="";
        String temp = "";
        String actual_Temp = "";
        //byte[] temp = new byt
        for(int i = 4; i<10;i++){
            temp += new String(new byte[] { record[i] },StandardCharsets.UTF_8)  ;
        }
        actual_Temp = DDLOperation.actualData(temp);
        if(!actual_Temp.equals("empty")){
            record_str += actual_Temp ;
        }
        temp = "";
        for(int i = 10; i<16;i++){
            temp += new String(new byte[] { record[i] },StandardCharsets.UTF_8)  ;
        }
        actual_Temp = DDLOperation.actualData(temp);

        if(!actual_Temp.equals("empty")){
            record_str +=  " " +actual_Temp ;
        }
        temp = "";
        for(int i = 16; i<22;i++){
            temp += new String(new byte[] { record[i] },StandardCharsets.UTF_8)  ;
        }
        actual_Temp = DDLOperation.actualData(temp);

        if(!actual_Temp.equals("empty")){
            record_str += " " +actual_Temp;
        }
        temp = "";
        for(int i = 22; i<28;i++){
            temp += new String(new byte[] { record[i] },StandardCharsets.UTF_8)  ;
        }
        actual_Temp = DDLOperation.actualData(temp);

        if(!actual_Temp.equals("empty")){
            record_str += " " +actual_Temp;
        }
        temp = "";
        for(int i = 28; i<34;i++){
            temp += new String(new byte[] { record[i] },StandardCharsets.UTF_8)  ;
        }
        actual_Temp = DDLOperation.actualData(temp);

        if(!actual_Temp.equals("empty")){
            record_str += " " +actual_Temp;
        }
        temp = "";
        for(int i = 34; i<40;i++){
            temp += new String(new byte[] { record[i] },StandardCharsets.UTF_8)  ;
        }
        actual_Temp = DDLOperation.actualData(temp);

        if(!actual_Temp.equals("empty")){
            record_str += " " +actual_Temp;
        }
        temp = "";

        return record_str;
    }
    public static int updateRecordPageHeader(int page_number,RandomAccessFile recordFile)throws IOException{
        byte[] numberOfRecord = new byte[4];
        recordFile.seek((page_number-1)*Constants.PAGESIZEFORAFILE + 4);
        recordFile.readFully(numberOfRecord);
        String strnumberOfRecord = new String(numberOfRecord,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        recordFile.seek((page_number-1)*Constants.PAGESIZEFORAFILE + 4);
        int numberOfRecordData = Integer.parseInt(DDLOperation.actualData(strnumberOfRecord));
        recordFile.writeBytes(DDLOperation.doPadding(""+(numberOfRecordData+1),4)); //next 2 bytes is number of pages
        return numberOfRecordData+1;
    
    }
    public static void updatePointerToRecord(int numberOfData, int page_number,RandomAccessFile recordFile)throws IOException{
        int nextRecordPointerData = 1;
        recordFile.seek((page_number-1)*Constants.PAGEHEADERSIZEFORAFILE);
        int locationOfAvailableRecord = 0;
        if(numberOfData >= 10){
            locationOfAvailableRecord = 0;
        }
        else{

            int nextRecord = 0;
            while(nextRecordPointerData != 0){
                recordFile.seek((page_number-1)*Constants.PAGESIZEFORAFILE+Constants.PAGEHEADERSIZEFORAFILE + nextRecord);
                byte[] nextRecordPointer = new byte[4];
                recordFile.readFully(nextRecordPointer);
                String strnextRecordPointer = new String(nextRecordPointer,StandardCharsets.UTF_8);
                nextRecordPointerData = Integer.parseInt(DDLOperation.actualData(strnextRecordPointer));
                locationOfAvailableRecord = (page_number-1)*Constants.PAGESIZEFORAFILE+Constants.PAGEHEADERSIZEFORAFILE + nextRecord;
                nextRecord += 40;
                //System.out.println("nextRecordPointerData " + nextRecordPointerData);
                //System.out.println("locationOfAvailableRecord " + locationOfAvailableRecord);
            }
            
        }
        recordFile.seek((page_number-1)*Constants.PAGESIZEFORAFILE + 8);
        recordFile.writeBytes(DDLOperation.doPadding(""+locationOfAvailableRecord,4)); //next 2 bytes is number of pages

    }
    public static int getNumberOfFile(String typeName,RandomAccessFile system)throws IOException{
        byte[] numberOfFileInfo = new byte[4];
        int positionOfType = DDLOperation.getPositionOfType(typeName,system);

        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 14);
        system.readFully(numberOfFileInfo);
        String strnumberOfFile = new String(numberOfFileInfo,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        int numberOfFile = Integer.parseInt(DDLOperation.actualData(strnumberOfFile));
        return numberOfFile;
    }
    public static int getNumberOfRecords(String typeName,RandomAccessFile system)throws IOException{
        byte[] numberOfRecordInfo = new byte[4];
        int positionOfType = DDLOperation.getPositionOfType(typeName,system);

        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 22);
        system.readFully(numberOfRecordInfo);
        String strnumberOfRecord = new String(numberOfRecordInfo,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        int numberOfRecord = Integer.parseInt(DDLOperation.actualData(strnumberOfRecord));
        return numberOfRecord;
    }
    public static int getNumberOfFields(String typeName,RandomAccessFile system)throws IOException{
        byte[] numberOfFieldInfo = new byte[4];
        int positionOfType = DDLOperation.getPositionOfType(typeName,system);

        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 26);
        system.readFully(numberOfFieldInfo);
        String strnumberOfField = new String(numberOfFieldInfo,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        int numberOfFields = Integer.parseInt(DDLOperation.actualData(strnumberOfField));
        return numberOfFields;
    }
    public static void updateSystemCatalogue(String typeName,int file_update,int page_update,int record_update,RandomAccessFile system)throws IOException{
        byte[] numberOfFileInfo = new byte[4];
        byte[] numberOfPageInfo = new byte[4];
        byte[] numberOfRecordInfo = new byte[4];
        int positionOfType = DDLOperation.getPositionOfType(typeName,system);

        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 14);
        system.readFully(numberOfFileInfo);
        String strnumberOfFile = new String(numberOfFileInfo,StandardCharsets.UTF_8);
        int numberOfFile = Integer.parseInt(DDLOperation.actualData(strnumberOfFile));
        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 14);
        system.writeBytes(DDLOperation.doPadding(""+(numberOfFile+file_update),Constants.NUMBEROFFILES));

        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 18);
        system.readFully(numberOfPageInfo);
        String strnumberOfPage = new String(numberOfPageInfo,StandardCharsets.UTF_8);
        int numberOfPage = Integer.parseInt(DDLOperation.actualData(strnumberOfPage));
        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 18);
            
        system.writeBytes(DDLOperation.doPadding(""+(numberOfPage+page_update),Constants.NUMBEROFPAGES));

        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 22);
        system.readFully(numberOfRecordInfo);
        String strnumberOfRecord = new String(numberOfRecordInfo,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        int numberOfRecord = Integer.parseInt(DDLOperation.actualData(strnumberOfRecord));
        system.seek(positionOfType*Constants.TYPEINFORMATION + 8 + 22);
        system.writeBytes(DDLOperation.doPadding(""+(numberOfRecord+record_update),Constants.NUMBEROFRECORDS));
        
    }

    public static void createNewPage(int nextPageInfo,int page_number,RandomAccessFile recordFile)throws IOException{
        recordFile.seek(nextPageInfo);
        recordFile.writeBytes(DDLOperation.doPadding(""+page_number,Constants.PAGEIDLENGTH));
        recordFile.writeBytes(DDLOperation.doPadding("0",Constants.PAGEIDLENGTH));
        recordFile.writeBytes(DDLOperation.doPadding(""+((page_number-1)*Constants.PAGESIZEFORAFILE+Constants.PAGEHEADERSIZEFORAFILE),6));
        recordFile.writeBytes(DDLOperation.doPadding(""+(page_number*Constants.PAGESIZEFORAFILE),6));
        for(int i=0;i<10;i++){
            recordFile.writeBytes("0   empty empty empty empty empty empty ");
        }
    }
    public static int getLocationFromKey(String[] splitted_line, RandomAccessFile recordFile, RandomAccessFile system)throws IOException{
        int locationOfAvailableRecord = 0;
        int numberOfPages = 1;
        int nextLocation = 20;
        byte[] RecordInfo = new byte[10];
        byte[] numberOfRecordInfo = new byte[4];
        byte[] locationOfNextPageInfo = new byte[6];
        String real_record = getRecordWithPrimaryKey(splitted_line,system);
        int strnumberOfRecordInfo_Data = 0;
    
        while(numberOfPages<= Constants.MAXNUMBEROFPAGES){ 
            //System.out.println(numberOfPages + "   " + nextLocation);
            recordFile.seek(nextLocation);
            recordFile.readFully(RecordInfo);
            String strRecordInfo = new String(RecordInfo,StandardCharsets.UTF_8);
            //System.out.println("1. " + strRecordInfo);
            //System.out.println("2. " + real_record);
            //System.out.println(strRecordInfo.equals(real_record) +"  "+ strnumberOfRecordInfo_Data);
            strnumberOfRecordInfo_Data ++ ;
            if(strRecordInfo.equals(real_record)){
                return nextLocation;
            }
            else{
                
                nextLocation += 40;
            }
            if(strnumberOfRecordInfo_Data>=10){
                recordFile.seek((numberOfPages-1)*Constants.PAGESIZEFORAFILE + 14);
                recordFile.readFully(locationOfNextPageInfo);
                String strnextPageInfo = new String(locationOfNextPageInfo,StandardCharsets.UTF_8);
                nextLocation = Integer.parseInt(DDLOperation.actualData(strnextPageInfo)) + 20;
                //System.out.println("next page: " + nextLocation);
                strnumberOfRecordInfo_Data= 0;
                numberOfPages++;
                recordFile.seek(nextLocation);
                if(recordFile.read()==-1){
                    break;
                }

            }
            
        }

        return -1;
    }

    public static int getRecordAll(int record_number,String[]record_array, String[] splitted_line, RandomAccessFile recordFile, RandomAccessFile system)throws IOException{
        int numberOfPages = 1;
        int nextLocation = 20;
        byte[] RecordInfo = new byte[40];
        byte[] numberOfRecordInfo = new byte[4];
        byte[] locationOfNextPageInfo = new byte[6];
        String real_record = "1   ";
        int strnumberOfRecordInfo_Data = 0;
    
        while(numberOfPages<= Constants.MAXNUMBEROFPAGES){ 
            //System.out.println(numberOfPages + "   " + nextLocation);
            recordFile.seek(nextLocation);
            recordFile.readFully(numberOfRecordInfo);
            String strRecordInfo = new String(numberOfRecordInfo,StandardCharsets.UTF_8);
            //System.out.println("1. " + strRecordInfo);
            //System.out.println("2. " + real_record);
            //System.out.println(strRecordInfo.equals(real_record) +"  "+ strnumberOfRecordInfo_Data);
            strnumberOfRecordInfo_Data ++ ;
            if(strRecordInfo.equals(real_record)){
                recordFile.seek(nextLocation);
                recordFile.readFully(RecordInfo);
                String strRecordInfo_full = new String(RecordInfo,StandardCharsets.UTF_8);
                record_array[record_number] = strRecordInfo_full;
                record_number ++;
                nextLocation += 40;
            }
            else{
                
                nextLocation += 40;
            }
            if(strnumberOfRecordInfo_Data>=10){
                recordFile.seek((numberOfPages-1)*Constants.PAGESIZEFORAFILE + 14);
                recordFile.readFully(locationOfNextPageInfo);
                String strnextPageInfo = new String(locationOfNextPageInfo,StandardCharsets.UTF_8);
                //System.out.println("strnextPageInfo " + strnextPageInfo + "  " + ((numberOfPages-1)*Constants.PAGESIZEFORAFILE + 14));
                nextLocation = Integer.parseInt(DDLOperation.actualData(strnextPageInfo)) + 20;
                //System.out.println("next page: " + nextLocation);
                strnumberOfRecordInfo_Data= 0;
                numberOfPages++;
                recordFile.seek(nextLocation);
                if(recordFile.read()==-1){
                    break;
                }

            }
            
        }

        return record_number;
    }
    public static String getRecordWithPrimaryKey(String[] splitted_line, RandomAccessFile system)throws IOException{
        String record = "1   ";
        record += (DDLOperation.doPadding(splitted_line[3], Constants.LENGTHOFONEFIELD));

        return record;

    
    }
    public static String getRecord(String[] splitted_line, RandomAccessFile system)throws IOException{
        String record = "1   ";
        String typeName = splitted_line[2];
        int numberOfFields = getNumberOfFields(typeName, system);

        for (int i = 0; i < Constants.MAXFIELDNUMBER; i++) {
            if(i<numberOfFields){
                record +=(DDLOperation.doPadding(splitted_line[i+3], Constants.LENGTHOFONEFIELD));
            }
            else{
                record +="empty ";
            }
        }
        return record;

    
    }
    
    public static int getAvailableLocation(String typeName,RandomAccessFile recordFile,RandomAccessFile system)throws IOException{
        int page_number = 1;
        byte[] numberOfRecordInfo = new byte[4];
        byte[] locationOfRecordInfo = new byte[6];
        byte[] locationOfNextPageInfo = new byte[6];
        int nextPageInfo = 4;
        while(page_number<= Constants.MAXNUMBEROFPAGES){ 
            recordFile.seek(nextPageInfo); //getAvailableRecord
            recordFile.readFully(numberOfRecordInfo);
            String strRecordInfo = new String(numberOfRecordInfo,StandardCharsets.UTF_8);
            //System.out.println("strRecordInfo "  + strRecordInfo );
            int recordInfo = Integer.parseInt(DDLOperation.actualData(strRecordInfo));
            if(recordInfo<10){
                recordFile.seek(nextPageInfo+4);
                recordFile.readFully(locationOfRecordInfo);
                String strLocationRecordInfo = new String(locationOfRecordInfo,StandardCharsets.UTF_8);
                //System.out.println("strNumberOfTypes "  + strLocationRecordInfo );
                int locationRecordInfo = Integer.parseInt(DDLOperation.actualData(strLocationRecordInfo));
                return locationRecordInfo;
            }else{
                page_number++;
                recordFile.seek(nextPageInfo+10);
                recordFile.readFully(locationOfNextPageInfo);
                String strLocationOfNextPageInfo = new String(locationOfNextPageInfo,StandardCharsets.UTF_8);
                //System.out.println("strNumberOfTypes "  + strLocationOfNextPageInfo );
                nextPageInfo = Integer.parseInt(DDLOperation.actualData(strLocationOfNextPageInfo));
                recordFile.seek(nextPageInfo);
                if(recordFile.read()==-1){
 
                    if(page_number<15){
                        //System.out.println(nextPageInfo+ "  "+page_number );
                        createNewPage(nextPageInfo,page_number,recordFile);
                        updateSystemCatalogue(typeName,0,1,0,system);
                    }
                }
                nextPageInfo = nextPageInfo + 4;
                //System.out.println(nextPageInfo);

            }

        }

        return -1;
    }

    public static void createRecord(String[] splitted_line, RandomAccessFile system) throws IOException {
        String typeName = splitted_line[2];
        if(DDLOperation.isExists(typeName, system)){
            boolean locationFound = false;
            int file_url = 1;
            int numberOfFile = getNumberOfFile(typeName,system);
            RandomAccessFile recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
            int nextAvailableLocation = 0;
            while(!locationFound){
                recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
                nextAvailableLocation = getAvailableLocation(typeName,recordFile,system);
                if(nextAvailableLocation == -1){
                    file_url ++;
                    if(file_url == numberOfFile){
                        DDLOperation.createTypeFile(typeName+"_"+file_url);
                        updateSystemCatalogue(typeName,1,1,0,system);

                    }
                }else{
                    locationFound = true;
                
                }
            }
            //System.out.println(nextAvailableLocation+ " nextAvailableLocation");
            recordFile.seek(nextAvailableLocation);
            int numberOfFields = getNumberOfFields(typeName, system);
            recordFile.writeBytes(DDLOperation.doPadding("1",Constants.ISEMPTYINFORMATION_RECORD)); //next 2 bytes is number of pages
            //System.out.println("third  " + doPadding("0",2) );
            //next 4 bytes are number of fields
            //next 84 bytes are page numbers which is empty now
            //system.seek(8 + (numOfTypes)*Constants.TYPEINFORMATION+98); //give 84 bytes break, start at 98th byte to write
            if(numberOfFields <= Constants.MAXFIELDNUMBER){
                for (int i = 0; i < Constants.MAXFIELDNUMBER; i++) {
                    if(i<numberOfFields){
                       recordFile.writeBytes(DDLOperation.doPadding(splitted_line[i+3], Constants.LENGTHOFONEFIELD));
                    }
                    else{
                       recordFile.writeBytes("empty ");
                    }
                }
               }else{
                //System.out.println("Max number of Field exceeded!");
            }
            updateSystemCatalogue(typeName, 0, 0, 1, system);
            
            int numberOfData =updateRecordPageHeader(nextAvailableLocation/Constants.PAGESIZEFORAFILE +1,recordFile);
            //System.out.println("numberOfData " + numberOfData);
            updatePointerToRecord(numberOfData,nextAvailableLocation/Constants.PAGESIZEFORAFILE + 1,recordFile);


            

        }
        
    }

    public static void updateRecord(String[] splitted_line, RandomAccessFile system) throws IOException {
        String typeName = splitted_line[2];
        if(DDLOperation.isExists(typeName, system)){

            
            int recordLocation = 0;
            boolean locationFound = false;
            int file_url = 1;
            int numberOfFile = getNumberOfFile(typeName,system);
            RandomAccessFile recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
            while(!locationFound){
                recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
                recordLocation = getLocationFromKey(splitted_line, recordFile, system);
                if(recordLocation == -1 && file_url<numberOfFile){
                    file_url ++;
                }else{
                    locationFound = true;
                
                }
            }
            recordFile.seek(recordLocation);
            recordFile.writeBytes(getRecord(splitted_line,system));
        }
    }
    public static void searchRecord(String[] splitted_line, RandomAccessFile system,BufferedWriter writer) throws IOException {
        String typeName = splitted_line[2];
        if(DDLOperation.isExists(typeName, system)){

            
            int recordLocation = 0;
            boolean locationFound = false;
            int file_url = 1;
            int numberOfFile = getNumberOfFile(typeName,system);
            RandomAccessFile recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
            while(!locationFound){
                recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
                recordLocation = getLocationFromKey(splitted_line, recordFile, system);
                if(recordLocation == -1 && file_url<numberOfFile){
                    file_url ++;
                }else{
                    locationFound = true;
                
                }
            }

            byte[] recordInfo = new byte[40];
            if(recordLocation != -1){
                recordFile.seek(recordLocation);
                recordFile.readFully(recordInfo);
                String strRecordInfo = new String(recordInfo,StandardCharsets.UTF_8);
                String record = record_byteToString(recordInfo);
                //System.out.println("search Record "  + record );
                writer.write(record + "\n");
            }


        }
    }
    public static void listRecord(String[] splitted_line, RandomAccessFile system,BufferedWriter writer) throws IOException {
        String typeName = splitted_line[2];
        if(DDLOperation.isExists(typeName, system)){

            
            boolean locationFound = false;
            int file_url = 1;
            int numberOfFile = getNumberOfFile(typeName,system);
            int numberOfRecord = getNumberOfRecords(typeName, system);
            String[] record_array = new String[numberOfRecord];
            RandomAccessFile recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
            int record_number = 0;
            while(!locationFound){
                recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
                record_number = getRecordAll(record_number,record_array,splitted_line, recordFile, system);
                if(record_number<numberOfRecord && file_url<numberOfFile){
                    file_url ++;
                }else{
                    locationFound = true;
                
                }
            }
            String[] record_array_str = new String[numberOfRecord];
            for(int i = 0; i<record_number;i++){
                //System.out.println(i+".  " + record_array[i]);
                String record = record_byteToString(record_array[i].getBytes());
                record_array_str[i] = record;
                //System.out.println("search Record "  + record );
                //writer.write(record + "\n");
            }
            sortRecords(record_array_str,record_number);
            for(int i = 0; i<record_number;i++){
                writer.write(record_array_str[i] + "\n");
            }

    }
    }

    public static void deleteRecord(String[] splitted_line, RandomAccessFile system) throws IOException {
        String typeName = splitted_line[2];
        if(DDLOperation.isExists(typeName, system)){
            int recordLocation = 0;
            boolean locationFound = false;
            int file_url = 1;
            int numberOfFile = getNumberOfFile(typeName,system);
            RandomAccessFile recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
            while(!locationFound){
                recordFile = new RandomAccessFile("./"+typeName+"_"+file_url+".dat", "rw");
                recordLocation = getLocationFromKey(splitted_line, recordFile, system);
                if(recordLocation == -1 && file_url<numberOfFile){
                    file_url ++;
                }else{
                    locationFound = true;
                
                }
            }
            byte[] pageInfo = new byte[4];
            int page_number = recordLocation/Constants.PAGESIZEFORAFILE +1;
            int page_info_location  = (page_number-1)*Constants.PAGESIZEFORAFILE ;

            recordFile.seek(page_info_location);
            recordFile.readFully(pageInfo);
            String strPageInfo = new String(pageInfo,StandardCharsets.UTF_8);
            int page_id = Integer.parseInt(DDLOperation.actualData(strPageInfo));

            recordFile.seek(page_info_location+4);
            recordFile.readFully(pageInfo);
            strPageInfo = new String(pageInfo,StandardCharsets.UTF_8);
            int number_of_records = Integer.parseInt(DDLOperation.actualData(strPageInfo));
            //System.out.println(" page_info_location " + page_info_location +" recordLocation " + recordLocation + "  number_of_records" + number_of_records + " page_id " + page_id );
            if(number_of_records-1 == 0 && page_id == Constants.NUMBEROFPAGES){
                recordFile.seek(recordLocation);
                recordFile.writeBytes("0   empty empty empty empty empty empty ");
                recordFile.seek(recordLocation);
                recordFile.writeBytes("0   empty empty empty empty empty empty ");
                recordFile.seek(page_info_location);
                recordFile.writeBytes("10  0   5900  6300  ");
                updateSystemCatalogue(typeName,0,-1,-1,system);
                //delete page
                //drop a record info and page info by one from system catalogue

            }
            else if(number_of_records-1 == 0 && page_id ==1){
                    FileInputStream instream = null;
                    FileOutputStream outstream = null;
                while(file_url+1 < numberOfFile){
                    try{
                        File infile =new File("./"+ typeName +"_"+ file_url + ".dat");
                        File outfile =new File("./"+ typeName +"_"+ (file_url+1) + ".dat");
            
                        instream = new FileInputStream(infile);
                        outstream = new FileOutputStream(outfile);
            
                        byte[] buffer = new byte[1024];
            
                        int length;
                        /*copying the contents from input stream to
                        * output stream using read and write methods
                        */
                        while ((length = instream.read(buffer)) > 0){
                            outstream.write(buffer, 0, length);
                        }
            
                        //Closing the input/output file streams
                        instream.close();
                        outstream.close();
            
                        //System.out.println("File copied successfully!!");
            
                    }catch(IOException ioe){
                        ioe.printStackTrace();
                    }
                    
                }
                    DDLOperation.deleteTypeFile(typeName+ "_" + numberOfFile);
                    DDLOperation.createTypeFile(typeName+ "_" + numberOfFile);
                    updateSystemCatalogue(typeName,0,0,-1,system);


                //delete file
                //drop a record info a page info and a file info from system catalogue
                //change other file's name if exists
            }else if(number_of_records-1 == 0 ){
                recordFile.seek(recordLocation);
                recordFile.writeBytes("0   empty empty empty empty empty empty ");
                recordFile.seek(page_info_location);
                int nextAvailableRecord = (page_number-1)*Constants.PAGESIZEFORAFILE +20;
                int nextAvailablePage = (page_number)*Constants.PAGESIZEFORAFILE;
                recordFile.writeBytes("10  "+ DDLOperation.doPadding(""+(number_of_records-1), 4)+ DDLOperation.doPadding(""+nextAvailableRecord, 6)+ DDLOperation.doPadding(""+nextAvailablePage, 6));
                updateSystemCatalogue(typeName,0,-1,-1,system);
                //delete page
                //drop a record info and page info by one from system catalogue
                //make page all empty and drop one page and one record info
            }
            else{
                recordFile.seek(recordLocation);
                recordFile.writeBytes("0   empty empty empty empty empty empty ");
                recordFile.seek(page_info_location);
                recordFile.writeBytes( DDLOperation.doPadding(""+page_id,4)+ DDLOperation.doPadding(""+(number_of_records-1), 4));
                updateSystemCatalogue(typeName,0,0,-1,system);
                updatePointerToRecord((number_of_records-1),page_number,recordFile);
                //drop one record info from page and from system catalogue and make that record info empty
            }

    }
    }


}