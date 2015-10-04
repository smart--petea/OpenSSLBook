openssl req -newkey rsa:1024 -sha1 -keyout clientkey.pem  -out clientreq.pem
openssl x509 -req -in clientreq.pem -sha1 -extensions usr_cert -CA root.pem -CAkey root.pem -CAcreateserial -out clientcert.pem
cat clientcert.pem clientkey.pem  rootcert.pem > client.pem
openssl x509 -subject -issuer -noout -in client.pem
