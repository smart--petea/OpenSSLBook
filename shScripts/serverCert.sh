CERTS_DIR=./certs
SERVERKEY="$CERTS_DIR/serverkey.pem"
SERVERREQ="$CERTS_DIR/serverreq.pem"
SERVER_CA_PEM="$CERTS_DIR/serverCA.pem"
ROOTCERT="$CERTS_DIR/rootcert.pem"
SERVER_PEM="$CERTS_DIR/server.pem"
SERVER_CA_CERT="$CERTS_DIR/serverCAcert.pem"
SERVER_CERT="$CERTS_DIR/servercert.pem"

openssl req -newkey rsa:1024 -sha1 -keyout $SERVERKEY  -out $SERVERREQ
openssl x509 -req -in $SERVERREQ -sha1 -extensions usr_cert -CA $SERVER_CA_PEM -CAkey $SERVER_CA_PEM -CAcreateserial -out $SERVER_CERT
cat $SERVER_CERT $SERVERKEY $SERVER_CA_CERT $ROOTCERT > $SERVER_PEM
openssl x509 -subject -issuer -noout -in $SERVER_PEM
