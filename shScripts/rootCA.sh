CERTS_DIR=./certs
ROOTKEY="$CERTS_DIR/rootkey.pem"
ROOTREQ="$CERTS_DIR/rootreq.pem"
ROOTCERT="$CERTS_DIR/rootcert.pem"
ROOTPEM="$CERTS_DIR/root.pem"


openssl req -newkey rsa:1024 -sha1 -keyout $ROOTKEY -out $ROOTREQ
openssl x509 -req -in $ROOTREQ -sha1 -extensions v3_ca -signkey $ROOTKEY -out $ROOTCERT
cat $ROOTCERT $ROOTKEY > $ROOTPEM
openssl x509 -subject -issuer -noout -in $ROOTPEM
