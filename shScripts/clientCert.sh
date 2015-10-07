CERTS_DIR=./certs
CLIENTREQ="$CERTS_DIR/clientreq.pem"
CLIENTKEY="$CERTS_DIR/clientkey.pem"
CLIENT="$CERTS_DIR/client.pem"
CLIENTCERT="$CERTS_DIR/clientcert.pem"
ROOTCERT="$CERTS_DIR/rootcert.pem"
ROOTPEM="$CERTS_DIR/root.pem"

openssl req -newkey rsa:1024 -sha1 -keyout $CLIENTKEY  -out $CLIENTREQ
openssl x509 -req -in $CLIENTREQ -sha1 -extensions usr_cert -CA $ROOTPEM -CAkey $ROOTPEM -CAcreateserial -out $CLIENTCERT
cat $CLIENTCERT $CLIENTKEY  $ROOTCERT > $CLIENT
openssl x509 -subject -issuer -noout -in $CLIENT
