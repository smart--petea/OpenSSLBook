openssl req -newkey rsa:1024 -sha1 -keyout serverkey.pem  -out serverreq.pem
openssl x509 -req -in serverreq.pem -sha1 -extensions usr_cert -CA serverCA.pem -CAkey serverCA.pem -CAcreateserial -out servercert.pem
cat servercert.pem serverkey.pem serverCAcert.pem rootcert.pem > server.pem
openssl x509 -subject -issuer -noout -in server.pem
