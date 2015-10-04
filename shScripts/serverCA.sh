#openssl req -newkey rsa:1024 -sha1 -keyout serverCAkey.pem  -out serverCAreq.pem
openssl x509 -req -in serverCAreq.pem -sha1 -extensions v3_ca -CA root.pem -CAkey root.pem -CAcreateserial -out serverCAcert.pem
cat serverCAcert.pem serverCAkey.pem rootcert.pem > serverCA.pem
openssl x509 -subject -issuer -noout -in serverCA.pem
