//Add libraries.
import java.io.File;
import java.util.*;
import java.io.*;
import java.io.PrintStream;

/**
 * The Vault class interacts with the user. Allow them to add users,
 * store data, and access data with proper authentication.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class Vault{

    /**
     * This method is main method of the Vault class and it used to
     * take input from the user and branch off and commit different
     * actions based on that input.
     * @param args Able to read in filenames, and the option to whether
     * user wants to add users or look at data.
     */
    public static void main(String[] args){

        //Checks argument input range for Vault. 
        if(args.length == 0 || args.length>2){
            System.out.println("usage: java Vault [-au] <filename>");
            System.exit(1);
        }       

        //Error checks the file
        if(args.length == 1){
            fileCheck(args[0]);
        }
        else if(args.length == 2 && args[0].equals("-au")){
            fileCheck(args[1]);
        }

        //User input for authentication.
        System.out.print("username: ");
        String username = System.console().readLine();
        System.out.print("password: ");
        char[] psswd = System.console().readPassword();

        //Branches to addUser option from command line input.
        if(args.length == 2 && args[0].equals("-au") ){
            addUser(username, new String (psswd), args[1]);
            System.exit(0);
        }

        //Declare Scanner.
        Scanner sc = null; 

        //Opens file from filename inputed in command line.
        try {
            sc = new Scanner(new File(args[0]));
        } catch(IOException e) { 
            System.out.println("Error! File '"+args[0]+
            "' could not be opened.");
            System.exit(1);
        }

        //Checks authentication based on username and password.
        try{
            if(authentication(sc, username, psswd)){
                System.out.println("Access granted!");
                String input[]={}; 
                
                //Opens for user input. Reads input and branches off based 
                //on input.
                while(true){
                    
                    System.out.print("> ");
                    input = System.console().readLine().split(" ");
                    
                    if(input[0].equals("labels")&&input.length ==1){
                        labels(args[0],username, new String(psswd));
                    }   
                    else if(input[0].equals("get")&&input.length == 2){
                        getLabel(args[0], username, new String(psswd), 
                        input[1]);
                    }
                    else if(input[0].equals("quit")&&input.length==1){
                        System.exit(0);
                    }  
                    else if(input[0].equals("add")&&input.length==4){
                        addData(args[0], username, new String(psswd), 
                        input[1], input[2], input[3]);
                    }
                    else{
                        String er ="";
                        for(int i = 0; i < input.length; i++){
                            if(input.length==1){
                            er = input[i];
                            }
                            else{
                                er = er + " " + input[i];
                            }
                        }
                        System.out.println("Unknown command '"+er+"'.");
                }
            }
        }
            else{
                System.out.println("Access denied!");
                System.exit(1);
            }
            }catch (NoSuchElementException e){
                System.out.println(e.getMessage());
                System.exit(1);
            }catch (Throwable t){} 
    }
    
    /**
     * This method takes inputs and hashes the message. It is based on the 
     * TestHashers class done in an earlier part of this project. 
     * @param alg Algorithm
     * @param mssg The message that is being hashed or password.
     * @param choice Used for error printing.
     * @return The hashed process.
     * @throws IndexOutOfBoundsException is thrown when wrong algorithm is sent.
     * @throws NoSuchElementException For character outside range.
     */
    public static String hashProcess(String alg, char[] mssg, int choice) 
    throws IndexOutOfBoundsException, NoSuchElementException{
        
        ArrayList<Hasher> E = new ArrayList<Hasher>();
        E.add(new ClearHash());
        E.add(new CaesarShift());
        E.add(new VigenereShift());

        int i = -1;
        try {
          while( !E.get(++i).getAlgName().equals(alg) ) ;
        } catch(IndexOutOfBoundsException e) {
          throw new NoSuchElementException("Error! Hash algorithm '"
           + alg +"' not supported.");
        }

        //Declares ciphertext and declares and initializes the i
        //nitializationVector.

        String initializationVector = "GO_NAVY_2018^mid";
        String ciphertext = "";
        
        // Encrypt, decrypt print sumamry of results
        try{
            E.get(i).init(mssg);
            ciphertext = E.get(i).hash(initializationVector);
            return ciphertext;
        }catch(IndexOutOfBoundsException e){
            if(choice == 1){
                String[] x = e.getMessage().split(" ");
                System.out.println("Error! Invalid symbol '"
                +x[1]+"' in password.");  
                System.exit(1);
            }
        }
        return ciphertext;
    }

    /**
     * This method runs the authentication process for the vault. 
     * @param sc Scanner, reads the file with usernames and passwords
     * @param username Username inputed by user
     * @param psswd Password inputed by user
     * @return Boolean, true if passes authentication
     * @throws IndexOutOfBoundsException from hashProcess 
     * @throws NoSuchElementException from hashProcess
     */
    public static boolean authentication(Scanner sc, String username,
     char[] psswd) throws IndexOutOfBoundsException, NoSuchElementException{
        
        boolean a = false;
        
        while(sc.hasNext() != false){
            
            String infoline = sc.nextLine();
            String[] info = infoline.split(" ");
            
            if(info[1].equals(username)){
                if(hashProcess(info[2], psswd, 0).equals(new String(info[3]))){
                    a = true;
                    break;
                }
            } 
        }

        return a;
    }
    
    /**
     * Checks the file, if it exists and the format.
     * @param filename Name of file inputed by user.
     */
    public static void fileCheck(String filename){
        
        //Declares Scanner
        Scanner sc = null;
        
        try {
            sc = new Scanner(new File(filename));
        } catch(IOException e) {
            System.out.println("Error! File '"+filename+
            "' could not be opened.");
            System.exit(1);
        }

        //Declares formatCheck, used to eventually
        //check the format from text file.
        
        String[] formatCheck;
    
        while(sc.hasNextLine()){
            
            formatCheck = sc.nextLine().split(" ");
            
            if(!(formatCheck[0].equals("user")||
            formatCheck[0].equals("data"))){
                System.out.println("Error! File '"+filename+
                "' improperly formatted.");
                System.exit(1);
            }
        }
        
        sc.close();    
    }

    /**
     * This method adds a user to the system.
     * @param username inputted by user
     * @param password inputted by user, password for the user being added
     * @param filename file that stores the users and data inputted by user
     */
    public static void addUser(String username, String password,
     String filename){
        
        //Asks user for Hash Algorithm and stores input.
        System.out.print("Hash algorithm: ");
        String hashalg = System.console().readLine();
        
        //User error check
        Scanner sc = null;
        try {
            sc = new Scanner(new File(filename));
        } catch(IOException e) {
            System.out.println("Error! File '"+filename+
            "' could not be opened.");
            System.exit(1);
        }
        
        //Declares line which is used to store input from file.
        String[] line;
        
        while(sc.hasNextLine()){
            
            line = sc.nextLine().split(" ");
            
            if(line[1].equals(username)){
                System.out.println("Error! Username '"+username+
                "' already in use.");
                System.exit(1);
            }
        }
        
        sc.close();
        
        //Declares PrintWriter
        PrintWriter pw = null;
        
        try {
            pw = new PrintWriter(new BufferedWriter(
                new FileWriter(filename, true)));
        } catch (FileNotFoundException fnfe) {
            fnfe.printStackTrace();
        }catch (IOException e){}

        try{
            pw.println("user "+username+ " " + hashalg + " " + 
            hashProcess(hashalg, password.toCharArray(), 1));
        }catch(Throwable t){
            System.out.println(t.getMessage());
            System.exit(1);
        }

        if (pw != null) pw.close();

        System.exit(0);
    }

    /**
     * This method returns the labels from the data in the file.
     * @param filename Filename inputted by user.
     * @param username Username inputted by user.
     * @param password Password inputted by user.
     */
    public static void labels(String filename, String username,
     String password){
        
        Scanner sc = null;
        
        try {
            sc = new Scanner(new File(filename));
        } catch(IOException e) {
            System.out.println("Error! File '"+filename+
            "' could not be opened.");
            System.exit(1);
        }

        String[] input;
        String[] output={};
        boolean err = false;

        while(sc.hasNextLine()){
            err = false;
            input = sc.nextLine().split(" ");
            if(input[0].equals("data")&&
            input[1].equals(username)){
                try{
                output = decryptData(input[2], password, input[3])
                .split("_", 2);
                    System.out.println(output[0]);
                } catch(Throwable t){
                    System.out.println("Error! Corrupted entry '"
                    +input[3]+"' in vault file.");
                }
            }
        }

    }

    /**
     * Decrypts the data using the cipher in the text file and password as key.
     * @param encalg Encryption algorithm read from file.
     * @param password Password from user used for decryption
     * @param message Encrypted message from file.
     * @return Returns decrypted message.
     * @throws Throwable Errors include unknown algorothm, 
     * character out of bounds, etc.
     */
    public static String decryptData(String encalg, String password,
     String message) throws Throwable{
        
        String hopefully = "";
        
        ArrayList<Encryptor> E = new ArrayList<Encryptor>();
        E.add(new Clear());
        E.add(new Caesar());
        E.add(new Vigenere());
        
        boolean underscore = false;
        
        int i = -1;
        try {
            while( !E.get(++i).getAlgName().equals(encalg) ) ;
        } catch(IndexOutOfBoundsException e) {
            throw new NoSuchElementException("Unknown algorithm '"
             + encalg +"'.");
        }
        try{
            E.get(i).init(password.toCharArray());
            hopefully = E.get(i).decrypt(message);
            for(int j = 0; j < hopefully.length(); j++){
                if(hopefully.charAt(j)=='_'){
                    underscore = true;
                    break;
                }
            }
            if(underscore==false){
                throw new Throwable();
            }
            return hopefully;
        }
        catch(IndexOutOfBoundsException e){
          throw e;
        }
    }
    
    /**
     * Decrypts the message and returns the label part. 
     * @param filename Filename from user.
     * @param username Username from user.
     * @param password Password from user.
     * @param label First part of the encrypted message.
     */
    public static void getLabel(String filename, String username, 
    String password, String label){
        
        boolean err = false;
        Scanner sc = null;
        
        try {
            sc = new Scanner(new File(filename));
        } catch(IOException e) {
            System.out.println("Error! File '"+filename+"' could not be opened.");
            System.exit(1);
        }

        String[] input;
        String[] message={};
       
        //GETS MESSAGE FROM THE FILE.

        while(sc.hasNextLine()){
            
            input = sc.nextLine().split(" ");
            
            if(input[0].equals("data")&&input[1].equals(username)){
                try{
                message = decryptData(input[2], password, input[3])
                .split("_", 2);
                } catch(Throwable t){
                    System.out.println("Error! Corrupted entry '"+input[3]+
                    "' in vault file.");
                    err =true;
                }
                if(message[0].equals(label)&&err==false){
                    System.out.println(message[1]);
                }
            }
        }
        
        sc.close(); 
    }

    /**
     * Adds Data to the file in the proper format.
     * @param filename From user.
     * @param username From user.
     * @param password From user, used for data encryption.
     * @param encalg From user, used for data encruption.
     * @param label First part of data.
     * @param text Second part of data.
     */
    public static void addData(String filename, String username,
     String password,String encalg, String label, String text){
        boolean copylabel = false;
        Scanner sc = null;
        String encryptedMessage = "";
        String message="";
        int counter = 0;
        int linecount = 0;
        String[] input;
        String[] dataSplit={};
        String filedatastore="";
        String[] fileDataStoreArray={};
        boolean err = false;

        //ERROR CHECKS

        if(!(encalg.equals("vigenere")||encalg.equals("caesar")
        ||encalg.equals("clear"))){
            System.out.println("Error! Encryption algorithm '"+
            encalg+"' not supported.");
            err = true;
        }

        for(int i = 0; i < label.length(); i++){
            if(((int)label.charAt(i)<42)||((int)label.charAt(i)>122)){
                System.out.println("Error! Label '"+label+"' is invalid.");
                err = true;
            }
        }

        for(int i = 0; i < text.length(); i++){
            if(((int)text.charAt(i)<42)||((int)text.charAt(i)>122)){
                System.out.println("Error! Invalid character '"+
                text.charAt(i)+"' in text.");
                err = true;
            }
        }
        
        //READS FILE

        if(err == false){

            try {
                sc = new Scanner(new File(filename));
            } catch(IOException e) {
                System.out.println("Error! File '"+filename+
                "' could not be opened.");
                System.exit(1);
            }
            while(sc.hasNextLine()){
                if(filedatastore.equals("")){
                    filedatastore = sc.nextLine();
                }else{
                    filedatastore = filedatastore + "\n" +
                     sc.nextLine();
                }
            }
        
            sc.close();
        
            try {
                sc = new Scanner(new File(filename));
            } catch(IOException e) { 
                System.out.println("Error! File '"+filename+
                "' could not be opened.");
                System.exit(1);
            }
        
            while(sc.hasNextLine()){
                input = sc.nextLine().split(" ");
                counter++;
                if(input[0].equals("data")&&input[1].equals(username)){
                    try{
                        dataSplit = decryptData(input[2], password,
                         input[3]).split("_", 2);
                    } catch(Throwable t){
                        System.out.println("Error! Corrupted entry '"
                        +input[3]+"' in vault file.");             
                    }
                    if(dataSplit[0].equals(label)){
                        copylabel = true;
                        linecount = counter;
                    }
                }   
            }
            
            sc.close();
            fileDataStoreArray = filedatastore.split("\n");
            
            try{
                encryptedMessage = encryptData(encalg, password, label
                 + "_" + text); 
            }catch(Throwable t){}
 
            //ADDS DATA

            PrintWriter pw = null;
            
            if(copylabel == false){
                try{
                    pw = new PrintWriter(new BufferedWriter
                    (new FileWriter(filename, true)));
                }catch (FileNotFoundException fnfe) {
                fnfe.printStackTrace();
                }catch (IOException e){}
                try{
                    pw.println("data "+username+ " " + encalg +
                     " " + encryptedMessage);
                }catch(Throwable t){
                    System.out.println(t.getMessage());
                }
            }
            else{
                try {
                    pw = new PrintWriter(new BufferedWriter
                    (new FileWriter(filename, true)));
                } catch (FileNotFoundException fnfe) {
                    fnfe.printStackTrace();
                }catch (IOException e){}

                for(int i = 0; i < linecount; i++){
                    pw.println(fileDataStoreArray[i]);
                }
                pw.println("data "+username+ " " + encalg +
                 " " + encryptedMessage);
                for(int i = linecount+1; i < fileDataStoreArray.length; i++){
                    pw.println(fileDataStoreArray[i]);
                }
            }
            
            if (pw != null) pw.close();
        }
    }
    
    /**
     * This method encrypts the Data to be placed in a file/ 
     * @param encalg From user.
     * @param password From user.
     * @param message From user.
     * @return Encrypted data.
     * @throws Throwable Errors include unknown algorothm, character
     *  out of bounds, etc.
     */
    public static String encryptData(String encalg, 
    String password, String message) throws Throwable{
        
        String hopefully = "";
       
        ArrayList<Encryptor> E = new ArrayList<Encryptor>();
        E.add(new Clear());
        E.add(new Caesar());
        E.add(new Vigenere());
       
        boolean underscore = false;
        int i = -1;

        try {
            while( !E.get(++i).getAlgName().equals(encalg) ) ;
        } catch(IndexOutOfBoundsException e) {
            throw new NoSuchElementException("Unknown algorithm '"
             + encalg +"'.");
        }
        
        try{
            E.get(i).init(password.toCharArray());
            hopefully = E.get(i).encrypt(message);
            return hopefully;
        }
        catch(IndexOutOfBoundsException e){
          throw e;
        }
    }
}