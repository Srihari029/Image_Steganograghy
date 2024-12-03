Image Steganography
Steganograpphy is a method of concealing data into image file or audio file. This can be used to monitor data forgery, message passing, etc.,
In this application we use LSB image steganography. The secret data is encode into the LSB bits of the image. Modifying LSB will have minimal effect on value,so we prefer LSB steganography.
The file containing secret data and image file must be passed through command line argument, since this application is console based application.
