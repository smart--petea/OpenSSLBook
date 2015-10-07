CERTS_DIR=./certs
SERVER_CA_KEY="$CERTS_DIR/serverCAkey.pem"
SERVER_CA_REQ="$CERTS_DIR/serverCAreq.pem"
ROOT_PEM="$CERTS_DIR/root.pem"
SERVER_CA_CERT="$CERTS_DIR/serverCAcert.pem"
ROOTCERT="$CERTS_DIR/rootcert.pem"
SERVER_CA_PEM="$CERTS_DIR/serverCA.pem"

openssl req -newkey rsa:1024 -sha1 -keyout $SERVER_CA_KEY  -out $SERVER_CA_REQ
openssl x509 -req -in $SERVER_CA_REQ -sha1 -extensions v3_ca -CA $ROOT_PEM -CAkey $ROOT_PEM -CAcreateserial -out $SERVER_CA_CERT
cat $SERVER_CA_CERT $SERVER_CA_KEY $ROOTCERT > $SERVER_CA_PEM
openssl x509 -subject -issuer -noout -in $SERVER_CA_PEM
