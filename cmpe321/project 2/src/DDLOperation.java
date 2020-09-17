import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.nio.file.*;
import java.util.Arrays;
/**
 * This class contains constant values which represent the constraints of database management system.
 */
public class DDLOperation {

    public static void deleteTypeFile(String typeName) throws IOException {
        Path fileToDeletePath = Paths.get("./"+typeName+".dat");
        Files.delete(fileToDeletePath);
    }

    public static String doPadding(String str, int maxLength) {
        int diff = maxLength - (str.length());
        if (diff < 0 ){
            //System.out.println("You can't have that many types!");
        }
        StringBuilder sb = new StringBuilder(maxLength);
        for (int i = 0; i < diff; i++) {
            sb.append(" ");
        }
        str += sb;
        return str;
    }
    public static String actualData(String fittedData){
        int dash = fittedData.indexOf(" ");
        // If there is no dash
        if(dash == -1) {
            return fittedData;
        }
        else {
            return fittedData.substring(0, fittedData.indexOf(" "));
        }
    }
    public static void createTypeFile(String fileName) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter("./"+fileName+".dat"));
        writer.write("1   0   20    420   ");
        for(int i=0;i<10;i++){
            writer.write("0   empty empty empty empty empty empty ");
        }
        writer.close();
    }
    /**
     * @return number of types in the system catalog
     */
    public static int getNumberOfTypes(RandomAccessFile system) throws IOException {
        byte[] numberOfTypes = new byte[4];
        system.seek(0);
        system.readFully(numberOfTypes);
        String strNumberOfTypes = new String(numberOfTypes,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        int numOfTypes = Integer.parseInt(actualData(strNumberOfTypes));
        return numOfTypes;
    }
    public static int getPositionOfType(String typeName,RandomAccessFile system) throws IOException {
        byte[] typenameInfo = new byte[10];
        String typeNameToBeDeleted = "";
        int location = 0 ;
        int numberOfTypes = getNumberOfTypes(system);
        while(!(typeNameToBeDeleted.equals(typeName)) && location<numberOfTypes){
            int newPosition = location*Constants.TYPEINFORMATION + 12 ;
            system.seek(newPosition);
            system.readFully(typenameInfo);
            String strTypeNameToBeDeleted = new String(typenameInfo,StandardCharsets.UTF_8);
            typeNameToBeDeleted = actualData(strTypeNameToBeDeleted);
            location++;

        }

        return location-1;
    }
    public static boolean isExists(String typeName,RandomAccessFile system) throws IOException {
        byte[] typenameInfo = new byte[10];
        String typeNameToBeDeleted = "";
        int location = 0 ;
        int numberOfTypes = getNumberOfTypes(system);
        while( location<numberOfTypes){
            int newPosition = location*Constants.TYPEINFORMATION + 12 ;
            system.seek(newPosition);
            system.readFully(typenameInfo);
            String strTypeNameToBeDeleted = new String(typenameInfo,StandardCharsets.UTF_8);
            typeNameToBeDeleted = actualData(strTypeNameToBeDeleted);
            location++;
            if(typeNameToBeDeleted.equals(typeName)){
                return true;
            }

        }

        return false;
    }

    public static int getNumberOfFiles(int locationOfType,RandomAccessFile system) throws IOException {
        byte[] fileInfo = new byte[4];
        system.seek(locationOfType*Constants.TYPEINFORMATION + 22);
        system.readFully(fileInfo);
        String strnumberOfFile = new String(fileInfo,StandardCharsets.UTF_8);
        //System.out.println("strNumberOfTypes "  + strNumberOfTypes );
        int numberOfFile = Integer.parseInt(actualData(strnumberOfFile));
        return numberOfFile;
    }
    // public static int getNextAvailablePoint(RandomAccessFile system,int numOfTypes) throws IOException {
    //     byte[] availabilityInfo = new byte[4];
    //     system.seek(8 + numOfTypes*Constants.TYPEINFORMATION);
    //     system.readFully(numberOfTypes);
    //     String strNumberOfTypes = new String(numberOfTypes,StandardCharsets.UTF_8);
    //     System.out.println("strNumberOfTypes "  + strNumberOfTypes );
    //     int numOfTypes = Integer.parseInt(actualData(strNumberOfTypes));
    //     return numOfTypes;
    // }
    /**
     * Creates a new type in system catalog.
     *
     * First, asks for the name of type. Then number of fields. Then name of the fields.
     */
    public static void createType(String[] splitted_line, RandomAccessFile system) throws IOException {
        if(!isExists(splitted_line[2], system)){
            int numOfTypes = getNumberOfTypes(system);
            //System.out.println("number of types " + numOfTypes );
            int newNumberOfTypes = numOfTypes + 1;
            int numberOfFields = Integer.parseInt(splitted_line[3]);
            // Update information for how many types there are
            system.seek(0);
            system.writeBytes(doPadding(Integer.toString(newNumberOfTypes),Constants.NUMBEROFTYPES_PAGEHEADER));
            //System.out.println("first " + doPadding(Integer.toString(newNumberOfTypes),4) );
            //add type info to system catalog
            //int nextAvailablePoint = getNextAvailablePoint(system, numOfTypes);
            system.seek(8 + (numOfTypes)*Constants.TYPEINFORMATION);
            //first 22 bytes is record header
            //4 bytes are type id
            system.writeBytes(doPadding(""+newNumberOfTypes,Constants.MAXTYPEIDLENGTH_RECORDHEADER)); 
            //System.out.println("second " + doPadding(splitted_line[2],Constants.MAXTYPELENGTH) );
            //10 bytes are typename
            system.writeBytes(doPadding(splitted_line[2],Constants.MAXTYPENAMELENGTH_RECORDHEADER)); //next 2 bytes is number of pages
            
            system.writeBytes(doPadding("1",Constants.NUMBEROFFILES));
            
            system.writeBytes(doPadding("1",Constants.NUMBEROFPAGES));
    
            system.writeBytes(doPadding("0",Constants.NUMBEROFRECORDS));
            //System.out.println("third  " + doPadding("0",2) );
            //next 4 bytes are number of fields
            //next 84 bytes are page numbers which is empty now
            //system.seek(8 + (numOfTypes)*Constants.TYPEINFORMATION+98); //give 84 bytes break, start at 98th byte to write
            if(numberOfFields <= Constants.MAXFIELDNUMBER){
                system.writeBytes(doPadding(Integer.toString(numberOfFields),Constants.MAXNUMBEROFFIELDS_RECORDHEADER)); //next 4 bytes is number of records
            }else{
                //System.out.println("Max number of Field exceeded!");
            }
            //System.out.println("fourth " + doPadding("0",4) );
            //system.writeBytes(doPadding(Integer.toString(numberOfFields),2)); //next 2 bytes is numberOfFields
            //System.out.println("fifth " + doPadding(Integer.toString(numberOfFields),2));
            // write name of fields
             for (int i = 0; i < Constants.MAXFIELDNUMBER; i++) {
                 if(i<numberOfFields){
                    system.writeBytes(doPadding(splitted_line[i+4], Constants.MAXFIELDLENGTH));
                 }
                 else{
                    system.writeBytes("empty     ");
                 }
             }
            String typeName = splitted_line[2];
            createTypeFile(typeName+"_1");
        }else{
            //System.out.println("This type exists!");
        }
        
    }

    public static void listAllTypes(String[] splitted_line, RandomAccessFile system,BufferedWriter writer) throws IOException {

            int numOfTypes = getNumberOfTypes(system);
            byte[] typeNameByte = new byte[Constants.MAXTYPENAMELENGTH_RECORDHEADER];
            String[] type_array = new String[numOfTypes];
            for(int i=0; i < numOfTypes; i++) {
                system.seek(8+(i*Constants.TYPEINFORMATION)+ Constants.MAXTYPEIDLENGTH_RECORDHEADER); //position where type info exists
                system.readFully(typeNameByte); //read type name
                String typeName = new String(typeNameByte,StandardCharsets.UTF_8);
                type_array[i] = typeName;
                //System.out.println("Type #"+(i+1)+": "+ actualData(typeName));
                
            }
            Arrays.sort(type_array);
            for(int i=0;i< numOfTypes;i++){
                writer.write(actualData(type_array[i]) +"\n");
            }
        
    }
        /**
     * Deletes a type from system catalog
     */
    public static void deleteType(String[] splitted_line, RandomAccessFile system) throws IOException {
            String typeName = splitted_line[2];
            int numberOfTypes = getNumberOfTypes(system);
            int positionToBeDeleted = getPositionOfType(typeName,system);

            int numberOfFiles = getNumberOfFiles(positionToBeDeleted,system);
            //System.out.println("typeName: " + typeName + "numberOfTypes: " + numberOfTypes +"position to be deleted: "+ positionToBeDeleted);
            while(positionToBeDeleted<numberOfTypes) {
                long positionOfDeleted = (positionToBeDeleted)*Constants.TYPEINFORMATION+8;
                long positionOfLast = (positionToBeDeleted+1)*Constants.TYPEINFORMATION+8;
                byte[] copyType = new byte[Constants.TYPEINFORMATION];
                system.seek(positionOfLast);
                if(system.read()==-1){
                    break;
                }
                system.seek(positionOfLast);
                system.readFully(copyType);
                system.seek(positionOfDeleted);
                system.write(copyType);
                positionToBeDeleted++;
            }

            // update number of types in system catalog
            system.seek(0);
            system.writeBytes(doPadding(""+(numberOfTypes-1),4));

            for(int i=1;i<=numberOfFiles;i++){
                deleteTypeFile(typeName+"_" + i);
            }
        }

}