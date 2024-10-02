/**
 * This is the Encryptor interface, function names are copied from project
 * website. I adjusted to allow the functions to throw exceptions.
 * @author MIDN 3/C Nicholas Zayfman
 */
public interface Encryptor {
  public String getAlgName();
  public void   init(char[] key) throws IndexOutOfBoundsException;
  public String encrypt(String plain) throws IndexOutOfBoundsException;
  public String decrypt(String cipher) throws IndexOutOfBoundsException;
}