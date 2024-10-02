//Add library.
import java.util.*;

/**
 * The TestEncryptor class is used to test the Caesar, Vigenere, and Clear
 * encryptor classes. 
 * The code is copied from the project website and adjusted in order to 
 * make it compatible with the different Encryptor classes I wrote.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class TestEncryptors {

  /**
   * This method is used to test the Caesar, Vigenere, and Clear hasher
   * classes.
   * @param args From command line - not used.
   * @throws Throwable when an unknown algorithm is inputted
   * from the user.
   */
  public static void main(String[] args) throws Throwable {
    
    // Create ArrayList of all supported encryptors
    ArrayList<Encryptor> E = new ArrayList<Encryptor>();
    E.add(new Clear());
    E.add(new Caesar());
    E.add(new Vigenere());


    // Get alg,psw,msg from user
    System.out.print("algorithm: ");
    String encalg = System.console().readLine();
    System.out.print("password : ");
    char[] password = System.console().readPassword();
    System.out.print("message  : ");
    String plaintext = System.console().readLine();

    // Find index of encryptor (throw exception if not found)
    int i = -1;
    try {
      while( !E.get(++i).getAlgName().equals(encalg) ) ;
    } catch(IndexOutOfBoundsException e) {
      throw new NoSuchElementException("Unknown algorithm '" + encalg +"'.");
    }

    // Encrypt, decrypt print summary of results
    try{
      E.get(i).init(password);
      String ciphertext = E.get(i).encrypt(plaintext);
      String hopefully = E.get(i).decrypt(ciphertext);
      System.out.println("plain : " + plaintext);
      System.out.println("cipher: " + ciphertext);
      System.out.println("decryp: " + hopefully);
    }
    catch(IndexOutOfBoundsException e){
      System.out.println("<Exception thrown out of main! Exact output not shown.>");
    }
  }
}