#include <iostream>
#include "Extern/cryptopp/aes.h"
#include "Extern/cryptopp/modes.h"
#include "Extern/cryptopp/filters.h"
#include "Extern/cryptopp/secblock.h"
#include "Extern/cryptopp/cryptlib.h"
#include "Extern/cryptopp/osrng.h"
#include "Extern/cryptopp/hex.h"
#include "Extern/cryptopp/files.h"
#include <filesystem>
#include "AESCryptConsole.h"

namespace fs = std::filesystem;

using namespace std;
using namespace Encryptor;
int main()
{
    FileEncryptor fe;
    string backup = "";
    cout << "did you backup the files you want to encrypt? (y/n)";
    getline(cin, backup);
    if (backup != "y")
    {
        cout << "backup your files and start again." << endl;
    }

    string dir = "";
    string extension = ".py";
    vector<string> filePaths;
    cout << "Enter root dir: ";
    getline(cin, dir);

    if (!dir.empty())
    {
        using namespace CryptoPP;

        //Setup key and IV generation
        AutoSeededRandomPool prng;
        SecByteBlock key(AES::MAX_KEYLENGTH);
        SecByteBlock iv(AES::BLOCKSIZE);

        //Generate key and IV
        prng.GenerateBlock(key, key.size());
        prng.GenerateBlock(iv, iv.size());

        //Get all python files from given dir
        //python files are found by checking each file's extension if it matches the value of variable 'extension'
        for (const auto & entry : fs::directory_iterator(dir))
        {
            if (fs::path(entry).extension() == extension)
            {
                try
                {
                    //initialize an empty string for the cipher text
                    string cipher = "";
                    //read the plain text from the curretn file
                    string plain = fe.readFile(entry);
                    //Encryption
                    CBC_Mode<AES>::Encryption enc;
                    enc.SetKeyWithIV(key, key.size(), iv);
                    StringSource s(fe.readFile(entry), true,
                        new StreamTransformationFilter(enc,
                            new StringSink(cipher),
                            StreamTransformationFilter::PKCS_PADDING
                        )
                    );

                    //encode the encrypted content to base16 data
                    string encodedCipher = "";
                    StringSource ss(cipher, true, new HexEncoder(new StringSink(encodedCipher)));

                    //replace the file's contents with the encoded cipher
                    ofstream ofs(fs::path(entry), ofstream::trunc);
                    ofs << encodedCipher;
                    ofs.close();
                }
                catch (const exception& e)
                {
                    cerr << e.what() << endl;
                }
            }
        }
        //output the encryption key and initialization vector, ready to be copied and pasted
        fe.outputKeyAndIV(key, iv);
    }
}
/// <summary>
/// Reads the file from the given path and returns it's contents as a string
/// </summary>
/// <param name="path">The file to be read</param>
/// <returns>File content as string</returns>
string FileEncryptor::readFile(fs::path path)
{
    ifstream ifs(path, ios::in | ios::binary);
    const auto fileSize = fs::file_size(path);
    string content(fileSize, '\0');
    ifs.read(content.data(), fileSize);
    return content;
}

/// <summary>
/// Outputs the encryption key and initialization vector as base16 encoded data
/// </summary>
/// <param name="key">The encryption key as bytes</param>
/// <param name="iv">The Initialization Vector as bytes</param>
void FileEncryptor::outputKeyAndIV(CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv)
{
    CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(std::cout));

    cout << "!!!Copy your key and Initialization Vector!!!\n\n" << endl;
    cout << "std::string base16Key = \"";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    cout << "\";" << endl;

    cout << "std::string base16IV = \"";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    cout << "\";" << endl;
}
