#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string PACKET_SEPARATOR = "::";

char hexToLetter(char ch)
{
    char lower = tolower(ch);

    if (lower >= '0' && lower <= '9')
    {
        int value = lower - '0';
        return 'A' + value;
    }
    else if (lower >= 'a' && lower <= 'f')
    {
        int value = lower - 'a';
        return 'K' + value;
    }

    return '?';
}

char letterToHex(char ch)
{
    char upper = toupper(ch);

    if (upper >= 'A' && upper <= 'J')
    {
        int value = upper - 'A';
        return '0' + value;
    }
    else if (upper >= 'K' && upper <= 'P')
    {
        int value = upper - 'K';
        return 'a' + value;
    }

    return '?';
}

string customHash(string input)
{
    reverse(input.begin(), input.end());

    unsigned long long hashValue = 68;

    for (int i = 0; i < input.length(); i++)
    {
        char currentChar = input[i];
        int asciiValue = (int)currentChar;

        hashValue = hashValue * 31 + asciiValue;
        hashValue = hashValue % (1ULL << 63);
    }

    char buffer[50];
    sprintf(buffer, "%llx", hashValue);

    string result = buffer;
    return result;
}

string hexToLetters(string hexText)
{
    string result = "";

    for (int i = 0; i < hexText.length(); i++)
    {
        char converted = hexToLetter(hexText[i]);
        result = result + converted;
    }

    return result;
}

string lettersToHex(string letterText)
{
    string result = "";

    for (int i = 0; i < letterText.length(); i++)
    {
        char converted = letterToHex(letterText[i]);

        if (converted == '?')
        {
            return "";
        }

        result = result + converted;
    }

    return result;
}

string buildKeyStream(string key, int requiredLength)
{
    string stream = "";

    for (int i = 0; i < key.length(); i++)
    {
        if (isalpha(key[i]))
        {
            stream = stream + (char)toupper(key[i]);
        }
    }

    if (stream.length() < requiredLength)
    {
        throw runtime_error("Key too short");
    }

    string finalStream = "";

    for (int i = 0; i < requiredLength; i++)
    {
        finalStream = finalStream + stream[i];
    }

    return finalStream;
}

string runningKeyEncrypt(string plainText, string key)
{
    int length = plainText.length();

    string keyStream = buildKeyStream(key, length);

    string encrypted = "";

    for (int i = 0; i < length; i++)
    {
        char p = plainText[i];
        char k = keyStream[i];

        int plainIndex = p - 'A';
        int keyIndex = k - 'A';

        int newIndex = (plainIndex + keyIndex) % 26;

        char encryptedChar = ALPHABET[newIndex];

        encrypted = encrypted + encryptedChar;
    }

    return encrypted;
}

string runningKeyDecrypt(string cipherText, string key)
{
    int length = cipherText.length();

    string keyStream;

    try
    {
        keyStream = buildKeyStream(key, length);
    }
    catch (...)
    {
        return "";
    }

    string decrypted = "";

    for (int i = 0; i < length; i++)
    {
        char c = cipherText[i];

        if (!isalpha(c))
        {
            return "";
        }

        char k = keyStream[i];

        int cipherIndex = c - 'A';
        int keyIndex = k - 'A';

        int newIndex = cipherIndex - keyIndex;

        if (newIndex < 0)
        {
            newIndex = newIndex + 26;
        }

        char decryptedChar = ALPHABET[newIndex];

        decrypted = decrypted + decryptedChar;
    }

    return decrypted;
}

string createPacket(string message, string encryptedHash)
{
    int length = message.length();

    string packet = to_string(length);
    packet = packet + PACKET_SEPARATOR;
    packet = packet + message;
    packet = packet + encryptedHash;

    return packet;
}

bool parsePacket(string packet, string &message, string &hash)
{
    int pos = packet.find(PACKET_SEPARATOR);

    if (pos == -1)
    {
        return false;
    }

    string lengthText = packet.substr(0, pos);
    int messageLength = stoi(lengthText);

    string remaining = packet.substr(pos + 2);

    if (messageLength > remaining.length())
    {
        return false;
    }

    message = remaining.substr(0, messageLength);
    hash = remaining.substr(messageLength);

    return true;
}

string sender(string message, string key)
{
    string hashValue = customHash(message);

    string letters = hexToLetters(hashValue);

    string encryptedHash = runningKeyEncrypt(letters, key);

    string packet = createPacket(message, encryptedHash);

    return packet;
}

bool receiver(string packet, string key)
{
    string message;
    string encryptedHash;

    bool valid = parsePacket(packet, message, encryptedHash);

    if (!valid)
    {
        cout << "[!] Invalid packet format\n";
        return false;
    }

    string decryptedLetters = runningKeyDecrypt(encryptedHash, key);

    if (decryptedLetters == "")
    {
        cout << "[!] Decryption failed\n";
        return false;
    }

    string hexValue = lettersToHex(decryptedLetters);

    if (hexValue == "")
    {
        cout << "[!] Conversion failed\n";
        return false;
    }

    string newHash = customHash(message);

    if (hexValue == newHash)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    while (true)
    {
        cout << "\nMENU\n";
        cout << "1. Send\n";
        cout << "2. Verify\n";
        cout << "3. Exit\n";

        cout << "Enter choice: ";
        string choice;
        getline(cin, choice);

        if (choice == "3")
        {
            cout << "Exiting...\n";
            break;
        }

        if (choice != "1" && choice != "2")
        {
            cout << "Invalid choice\n";
            continue;
        }

        cout << "Enter key: ";
        string key;
        getline(cin, key);

        if (key == "")
        {
            cout << "[!] Key cannot be empty\n";
            continue;
        }

        if (choice == "1")
        {
            cout << "Enter message: ";
            string message;
            getline(cin, message);

            try
            {
                string packet = sender(message, key);
                cout << "Packet: " << packet << endl;
            }
            catch (...)
            {
                cout << "[!] Key too short\n";
            }
        }
        else
        {
            cout << "Enter packet: ";
            string packet;
            getline(cin, packet);

            bool result = receiver(packet, key);

            if (result)
            {
                cout << "Authentication SUCCESS\n";
            }
            else
            {
                cout << "Authentication FAILED\n";
            }
        }
    }

    return 0;
}
