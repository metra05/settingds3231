# settingds3231
seting ds3231 dengan menggunakan esp8266 sebagai ap

#sebab
berarwal dari pembuatan jam jadwal sholat di mushola mushola, dimana untuk menekan biaya maka digunakan atmega328 
sebagai microkontrolernya tanpa ada menu untuk menge-set waktunya. sehingga muncul kesulitan ketika ada perubahan pada jam yang tampil.
yang terjadi ketika ada psu yang short ataupun masalah yang lain

#solusi
agar tidak terlalu ribet untuk membawa laptop ketika digunakan hanya untuk meng-seting waktu pada sebuah modul ds3231 maka dibuatlah
handheld yang sederhana dengan memanfaatkan esp8266, yang bekerja sebagai access point untuk selanjutnya menampilkan ui 
yang mengirimkan setingan waktu baru ke modul ds3231. 