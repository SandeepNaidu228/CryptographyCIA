# Hash Function with Running Cipher  

**M V N Sandeep Naidu**  
**23011102045**  
**3rd Year, B.Tech CSE (IoT)-A**

---

## Hash Function  

In this implementation, a simple custom hash function is used to generate a fixed-size output from any given message. The idea is to process each character using its ASCII value and continuously update a hash value.

The process starts by reversing the input string. This adds a small variation so that even similar inputs can produce different hashes.

For each character in the reversed string:
- The initial hash value is taken as **68** (ASCII value of 'D')  
- The current hash is multiplied by **31**  
- The ASCII value of the character is added  
- The result is reduced using modulo **2⁶⁴** to keep it within limits  

### Formula used:

<p align="center">
  <b><code>hash = (hash × 31 + ASCII(character)) mod 2^64</code></b>
</p>

---

## Running Key Cipher (Encryption)  

After generating the hash, it is encrypted using a running key cipher.

In this method:
- A key is provided by the user  
- Only alphabetic characters from the key are considered  
- Each character of the hash (after conversion) is combined with the corresponding character from the key  

Encryption is done by shifting characters forward, while decryption shifts them backward. This is similar to a Vigenère-style approach.

---

## Overall Working  

The full process works in two stages:

1. The input message is passed through the hash function to generate a hash value  
2. The hash is converted into letters and encrypted using the running key cipher  
3. The final output packet contains both the original message and the encrypted hash  

At the receiver side, the same key is used to decrypt the hash and verify whether the message has been altered.

---

## Instructions to Run  

- Select **1 (Send)** from the menu  
- Enter the running key  
- Enter the message  
- The program will generate a packet containing the message and encrypted hash  

For verification:
- Select **2 (Verify)**  
- Enter the same key  
- Paste the received packet (copy-paste is recommended to avoid mistakes)  

---

## Example 1  

**Running Key:** ElliotSimpsons  
**Message:** HelloWorld  

**Output:**  
10::HelloWorldQAWVBWXLOBDTRW


---

## Example 2  

**Running Key:** LockAndKey  
**Message:** abort  

**Output:**  
5::abortSYMLKCRW
