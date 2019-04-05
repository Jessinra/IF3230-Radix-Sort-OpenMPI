# OpenMPI - Radix Sort
## IF3230 Sistem Paralel dan Terdistribusi

Kelompok kami menggunakan sample input dengan N = 5000, 50000, 100000, 200000, 400000 dengan jumlah block B 8. Selain itu, kelompok kami juga menggunakan varian processor P 4 dan 8 karena diharapkan dapat digunakan untuk menganalisa kinerja paralelisme juga thread mana yang lebih baik digunakan.

Keterangan:
P = jumlah processor
N = jumlah elemen pada array
B = jumlah block yang digunakan

#### [Sample Input dan Output]
##### Untuk N = 400000
Serial<br>
![](../pictures/Serial_400000_block8.png)

Paralel dengan 4 Processor<br>
![](../pictures/Paralel4_400000_block8.png)

Paralel dengan 8 Processor<br>
![](../pictures/Paralel8_400000_block8.png)

##### Untuk N = 200000
Serial<br>
![](../pictures/Serial_200000_block8.png)

Paralel dengan 4 Processor<br>
![](../pictures/Paralel4_200000_block8.png)

Paralel dengan 8 Processor<br>
![](../pictures/Paralel8_200000_block8.png)

##### Untuk N = 100000
Serial<br>
![](../pictures/Serial_100000_block8.png)

Paralel dengan 4 Processor<br>
![](../pictures/Paralel4_100000_block8.png)

Paralel dengan 8 Processor<br>
![](../pictures/Paralel8_100000_block8.png)

##### Untuk N = 50000
Serial<br>
![](../pictures/Serial_50000_block8.png)

Paralel dengan 4 Processor<br>
![](../pictures/Paralel4_50000_block8.png)

Paralel dengan 8 Processor<br>
![](../pictures/Paralel8_50000_block8.png)

##### Untuk N = 5000
Serial<br>
![](../pictures/Serial_5000_block8.png)

Paralel dengan 4 Processor<br>
![](../pictures/Paralel4_5000_block8.png)

Paralel dengan 8 Processor<br>
![](../pictures/Paralel8_5000_block8.png)