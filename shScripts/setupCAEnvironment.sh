EXAMPLECA=/opt/exampleca
mkdir $EXAMPLECA

CERTS="$EXAMPLECA/certs"
PRIVATE="$EXAMPLECA/private"
SERIAL="$EXAMPLECA/serial"
INDEX_TXT="$EXAMPLECA/index.txt"
mkdir $CERTS $PRIVATE
chmod g-rwx,o-rwx $PRIVATE
echo '01' > $SERIAL
touch $INDEX_TXT

OPENSSL_FILE=openssl.cnf
OPENSSL_CONF="$EXAMPLECA/$OPENSSL_FILE"
cp "shScripts/$OPENSSL_FILE" $OPENSSL_CONF
export OPENSSL_CONF