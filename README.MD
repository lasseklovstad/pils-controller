# Pils temperature controller

# Secrets
Copy `secrets.template.h` into `include` directory and rename to `secrets.h`. Fill in the required secrets from the website.

# Certificate
Download the certificate from https://letsencrypt.org/certs/isrgrootx1.pem and create a new file in `data/isgrootx1.pem`.
This file must then be uploaded to the esp32. WIth the command: `Upload Filesystem Image`

# Setup Windows for testing
1. Last ned https://github.com/niXman/mingw-builds-binaries?tab=readme-ov-file
2. Installer til `C:\CMinGW-w64`
3. Legg til Miljøvariabler til PATHS : `C:\CMinGW-w64\mingw64\bin`
4. Verifiser installasjon: `gcc --version` `g++ --version`
5. Legg til platform IO `pio` i miljøvariabler PATHS: `%USERPROFILE%\.platformio\penv\Scripts`
6. Kjør tester med `pio test -e native`

Vi bruker https://github.com/eranpeer/FakeIt/wiki/Quickstart