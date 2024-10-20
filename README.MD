# Pils temperature controller

# Secrets
Copy `secrets.template.h` into `include` directory and rename to `secrets.h`. Fill in the required secrets from the website.

# Certificate
Download the certificate from https://letsencrypt.org/certs/isrgrootx1.pem and create a new file in `data/isgrootx1.pem`.
This file must then be uploaded to the esp32. WIth the command: `Upload Filesystem Image`