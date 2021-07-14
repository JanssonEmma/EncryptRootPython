#pragma once

#include <filesystem>

namespace Encryptor
{
	class FileEncryptor
	{
		public:
			FileEncryptor();
			~FileEncryptor();
			std::string readFile(std::filesystem::path path);
			void outputKeyAndIV(CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv);
	};

	FileEncryptor::FileEncryptor()
	{
	}

	FileEncryptor::~FileEncryptor()
	{
	}
}

	