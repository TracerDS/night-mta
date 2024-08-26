#include <vendors/OpenSSL/include/openssl/evp.h>
#include <vendors/OpenSSL/include/openssl/aes.h>
#include <vendors/OpenSSL/include/openssl/rand.h>
#include <vendors/OpenSSL/include/openssl/rsa.h>
#include <string>

#include <Shared/sdk/SharedUtils/Misc.hpp>
#include <Shared/sdk/SharedUtils/Random.hpp>

namespace NightMTA::Shared::Crypto {
    namespace aes {
        inline std::string generateRandomIV() noexcept {
            std::string iv(16, 0);
            for (auto i = 0; i < 16; i++) {
                iv[i] = Random::RandomInRange(0x0, 0xF);
            }

            return iv;
        }

        inline std::vector<std::uint8_t> encodeAES (
            const std::vector<std::uint8_t>& input,
            const std::vector<std::uint8_t>& key,
            const std::vector<std::uint8_t>& iv,
            const EVP_CIPHER* method
        ) noexcept {
            if (!method)
                return {};

            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx)
                return {};

            if (EVP_EncryptInit_ex(ctx, method, nullptr, key.data(), iv.data()) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                return {};
            }

            std::vector<std::uint8_t> ciphertext;
            ciphertext.resize(input.size() + EVP_CIPHER_block_size(method));

            int len;
            if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, input.data(),
                static_cast<int>(input.size())
            ) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                return {};
            }

            int ciphertext_len = len;

            if (EVP_EncryptFinal_ex(ctx, &ciphertext[0] + len, &len) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                return {};
            }

            ciphertext_len += len;
            ciphertext.resize(ciphertext_len);

            EVP_CIPHER_CTX_free(ctx);

            return ciphertext;
        }

        inline std::vector<std::uint8_t> encodeAES (
            const std::string& input,
            const std::string& key,
            const std::string& iv,
            const EVP_CIPHER* method
        ) noexcept {
            return encodeAES (
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                std::vector<std::uint8_t>(key.cbegin(), key.cend()),
                std::vector<std::uint8_t>(iv.cbegin(), iv.cend()),
                method
            );
        }

        inline std::string encodeAESString(
            const std::vector<std::uint8_t>& input,
            const std::vector<std::uint8_t>& key,
            const std::vector<std::uint8_t>& iv,
            const EVP_CIPHER* method
        ) noexcept {
            return Misc::BytesToHex(encodeAES(input, key, iv, method));
        }

        inline std::string encodeAESString (
            const std::string& input,
            const std::string& key,
            const std::string& iv,
            const EVP_CIPHER* method
        ) noexcept {
            return encodeAESString (
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                std::vector<std::uint8_t>(key.cbegin(), key.cend()),
                std::vector<std::uint8_t>(iv.cbegin(), iv.cend()),
                method
            );
        }

        inline std::vector<std::uint8_t> encodeAES128 (
            const std::string& input,
            const std::string& key,
            const std::string& iv = generateRandomIV()
        ) noexcept {
            static const auto method = EVP_aes_128_cbc();
            return encodeAES(input, key, iv, method);
        }

        inline std::string encodeAES128String (
            const std::string& input,
            const std::string& key,
            const std::string& iv = generateRandomIV()
        ) noexcept {
            static const auto method = EVP_aes_128_cbc();
            return encodeAESString(input, key, iv, method);
        }

        inline std::vector<std::uint8_t> encodeAES192(
            const std::string& input,
            const std::string& key,
            const std::string& iv = generateRandomIV()
        ) noexcept {
            static const auto method = EVP_aes_192_cbc();
            return encodeAES(input, key, iv, method);
        }

        inline std::vector<std::uint8_t> encodeAES256(
            const std::string& input,
            const std::string& key,
            const std::string& iv = generateRandomIV()
        ) noexcept {
            static const auto method = EVP_aes_256_cbc();
            return encodeAES(input, key, iv, method);
        }
    }

    namespace rsa {
        inline std::pair<EVP_PKEY*, EVP_PKEY*> GenerateKeyPair(std::uint32_t bits) noexcept {
            EVP_PKEY* privateKey = nullptr;
            EVP_PKEY* publicKey = nullptr;

            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
            if (!ctx)
                return {};

            if (EVP_PKEY_keygen_init(ctx) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            // Set the RSA key length
            if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            // Generate the private key
            if (EVP_PKEY_keygen(ctx, &privateKey) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            // Extract the public key from the private key
            publicKey = EVP_PKEY_new();
            if (!publicKey || !EVP_PKEY_copy_parameters(publicKey, privateKey)) {
                EVP_PKEY_free(privateKey);
                EVP_PKEY_free(publicKey);
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            EVP_PKEY_CTX_free(ctx);

            return { publicKey, privateKey };
        }

        inline std::vector<std::uint8_t> rsaEncrypt(
            const std::vector<std::uint8_t>& input,
            EVP_PKEY* publicKey
        ) noexcept {
            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(publicKey, nullptr);
            if (!ctx)
                return {};

            if (EVP_PKEY_encrypt_init(ctx) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            // Determine the buffer length for encryption
            std::size_t outLen;
            if (EVP_PKEY_encrypt(ctx, nullptr, &outLen, input.data(), input.size()) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            std::vector<std::uint8_t> encryptedData(outLen);

            // Perform the encryption
            if (EVP_PKEY_encrypt(ctx, encryptedData.data(), &outLen, input.data(), input.size()) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            EVP_PKEY_CTX_free(ctx);
            return encryptedData;
        }

        inline std::vector<std::uint8_t> rsaEncrypt (
            const std::string& input,
            EVP_PKEY* publicKey
        ) noexcept {
            return rsaEncrypt (
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                publicKey
            );
        }

        inline std::string rsaEncryptString (
            const std::vector<std::uint8_t>& input,
            EVP_PKEY* publicKey
        ) noexcept {
            return Misc::BytesToHex(rsaEncrypt(input, publicKey ));
        }

        inline std::string rsaEncryptString (
            const std::string& input,
            EVP_PKEY* publicKey
        ) noexcept {
            return rsaEncryptString(
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                publicKey
            );
        }

        std::vector<std::uint8_t> rsaDecrypt (
            const std::vector<std::uint8_t>& input,
            EVP_PKEY* privateKey
        ) {
            EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privateKey, nullptr);
            if (!ctx)
                return {};

            if (EVP_PKEY_decrypt_init(ctx) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            // Determine the buffer length for decryption
            std::size_t outLen;
            if (EVP_PKEY_decrypt(ctx, nullptr, &outLen, input.data(), input.size()) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            std::vector<std::uint8_t> decrypted(outLen);

            // Perform the decryption
            if (EVP_PKEY_decrypt(ctx, decrypted.data(), &outLen, input.data(), input.size()) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                return {};
            }

            EVP_PKEY_CTX_free(ctx);
            return decrypted;
        }

        inline std::vector<std::uint8_t> rsaDecrypt (
            const std::string& input,
            EVP_PKEY* privateKey
        ) noexcept {
            return rsaDecrypt(
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                privateKey
            );
        }

        inline std::string rsaDecryptString(
            const std::vector<std::uint8_t>& input,
            EVP_PKEY* privateKey
        ) noexcept {
            return Misc::BytesToHex(rsaDecrypt(input, privateKey));
        }

        inline std::string rsaDecryptString(
            const std::string& input,
            EVP_PKEY* privateKey
        ) noexcept {
            return rsaDecryptString (
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                privateKey
            );
        }
    }

    namespace sha {
        inline std::string hashString(
            const std::vector<std::uint8_t>& input,
            const EVP_MD* method
        ) noexcept {
            EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
            if (!mdctx)
                return {};

            if (EVP_DigestInit_ex(mdctx, method, nullptr) != 1) {
                EVP_MD_CTX_free(mdctx);
                return {};
            }

            if (EVP_DigestUpdate(mdctx, input.data(), input.size()) != 1) {
                EVP_MD_CTX_free(mdctx);
                return {};
            }

            // Finalize the digest and get the output
            std::uint8_t hash[EVP_MAX_MD_SIZE]{ 0 };
            std::uint32_t hashLength = 0;
            if (EVP_DigestFinal_ex(mdctx, hash, &hashLength) != 1) {
                EVP_MD_CTX_free(mdctx);
                return {};
            }

            // Clean up
            EVP_MD_CTX_free(mdctx);

            auto hex = Misc::BytesToHex(hash, EVP_MAX_MD_SIZE);
            hex.resize(hashLength);
            return hex;
        }

        inline std::string hashString(
            const std::string& input,
            const EVP_MD* method
        ) noexcept {
            return hashString(
                std::vector<std::uint8_t>(input.cbegin(), input.cend()),
                method
            );
        }

        inline std::string hashSha224(const std::vector<std::uint8_t>& input) noexcept {
            static const auto method = EVP_sha224();
            return hashString(input, method);
        }
        inline std::string hashSha224(const std::string& input) noexcept {
            return hashSha224(std::vector<std::uint8_t>(input.cbegin(), input.cend()));
        }

        inline std::string hashSha256(const std::vector<std::uint8_t>& input) noexcept {
            static const auto method = EVP_sha256();
            return hashString(input, method);
        }
        inline std::string hashSha256(const std::string& input) noexcept {
            return hashSha256(std::vector<std::uint8_t>(input.cbegin(), input.cend()));
        }

        inline std::string hashSha384(const std::vector<std::uint8_t>& input) noexcept {
            static const auto method = EVP_sha384();
            return hashString(input, method);
        }
        inline std::string hashSha384(const std::string& input) noexcept {
            return hashSha384(std::vector<std::uint8_t>(input.cbegin(), input.cend()));
        }

        inline std::string hashSha512(const std::vector<std::uint8_t>& input) noexcept {
            static const auto method = EVP_sha512();
            return hashString(input, method);
        }
        inline std::string hashSha512(const std::string& input) noexcept {
            return hashSha512(std::vector<std::uint8_t>(input.cbegin(), input.cend()));
        }

        inline std::string hashSha512_224(const std::vector<std::uint8_t>& input) noexcept {
            static const auto method = EVP_sha512_224();
            return hashString(input, method);
        }
        inline std::string hashSha512_224(const std::string& input) noexcept {
            return hashSha512_224(std::vector<std::uint8_t>(input.cbegin(), input.cend()));
        }

        inline std::string hashSha512_256(const std::vector<std::uint8_t>& input) noexcept {
            static const auto method = EVP_sha512_256();
            return hashString(input, method);
        }
        inline std::string hashSha512_256(const std::string& input) noexcept {
            return hashSha512_256(std::vector<std::uint8_t>(input.cbegin(), input.cend()));
        }
    }
}
