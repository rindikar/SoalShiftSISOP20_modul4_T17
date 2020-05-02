# SoalShiftSISOP20_modul4_T17
Nama Anggota Kelompok T17 :
  1. Faza Murtadho [05311840000034]
  2. Rindi Kartika Sari [05311840000013]

## Soal Shift Modul 4 dan Penyelesaian Soal Shift Modul 4
### Soal 1
Jasir, pekerja baru yang jenius, akan membuat program yang mengimplementasikan 2 (dua) buah metode enkripsi yang berjalan pada _directory_. Pada metode enkripsi pertama __"encv1 _ "__ dirancang dengan detail filesystem berikut ini :
  * Jika terdapat sebuah _directory_ yang dibuat dan di-_rename_ dengan awalan kata __"encv1 _ "__, maka _directory_ tersebut akan terenkripsi menggunakan metode enkripsi pertama __"encv1 _ "__. 
  * Jika terdapat sebuah _directory_ terenkripsi yang di-_rename_, maka _directory_ tersebut menjadi tidak terenkripsi. 
  * Setiap pembuatan _directory_ terenkripsi baru (__mkdir__ ataupun __rename__) akan tercatat ke sebuah database/log dalam file. 
  * Proses pengenkripsian dalam metode enkripsi pertama ini menggunakan metode enkripsi _caesar cipher_ dengan _key_ 
    ```bash 
    9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO
    ```
  * Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya. <br>
__Note:__ Metode enkripsi pertama ini mengabaikan karakter ‘/’ pada penamaan file dan ekstensi dari file. 
  #### Code :
  #### Penyelesaian :
  Untuk mendukung pembuatan program mengenai metode enkripsi tersebut, maka kita harus membuat fungsi __system call__ yang bertujuan untuk mengatur 3 (tiga) keperluan tertentu sesuai permintaan soal :
  * ```mkdir``` : kondisi dimana terjadi pembuatan _directory_ yang telah ditentukan.
  * ```create``` : kondisi dimana terjadi proses _rename_ pada _directory_ sesuai dengan nama yang telah ditentukan.
  * ```write``` : kondisi dimana terjadi penyimpanan perubahan pada database/log pada file. <br>
  
Oleh karena itu, untuk setiap __system call__ yang telah ditentukan maka kita memanggil 3 (tiga) fungsi antara lain :
  * Fungsi pertama adalah fungsi __changePath()__
    ```bash
      void changePath(char *fpath, const char *path, int isWriteOper, int isFileAsked) {
      char *ptr = strstr(path, "/encv1_");
      int state = 0;
      if (ptr != NULL) {
        if (strstr(ptr+1, "/") != NULL) state = 1;
      }
      char fixPath[1000];
      memset(fixPath, 0, sizeof(fixPath));
      if (ptr != NULL && state) {
        ptr = strstr(ptr+1, "/");
        char pathEncvDirBuff[1000];
        char pathEncryptedBuff[1000];
        strcpy(pathEncryptedBuff, ptr);
        strncpy(pathEncvDirBuff, path, ptr-path);
        if (isWriteOper) {
          char pathFileBuff[1000];
          char pathDirBuff[1000];
          getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
          decrypt(pathDirBuff, 0);
          sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
        } else if (isFileAsked) {
          char pathFileBuff[1000];
          char pathDirBuff[1000];
          char pathExtBuff[1000];
          getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
          char *whereIsExtension = strrchr(pathFileBuff, '.');
          if (whereIsExtension-pathFileBuff <= 1) {
            decrypt(pathDirBuff, 0);
            decrypt(pathFileBuff, 0);
            sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
          } else {
            char pathJustFileBuff[1000];
            memset(pathJustFileBuff, 0, sizeof(pathJustFileBuff));
            strcpy(pathExtBuff, whereIsExtension);
            snprintf(pathJustFileBuff, whereIsExtension-pathFileBuff+1, "%s", pathFileBuff);
            decrypt(pathDirBuff, 0);
            decrypt(pathJustFileBuff, 0);
            sprintf(fixPath, "%s%s/%s%s", pathEncvDirBuff, pathDirBuff, pathJustFileBuff, pathExtBuff);
          }
        } else {
          decrypt(pathEncryptedBuff, 0);
          sprintf(fixPath, "%s%s", pathEncvDirBuff, pathEncryptedBuff);
        }
      } else {
        strcpy(fixPath, path);
      }
      if (strcmp(path, "/") == 0) {
        sprintf(fpath, "%s", dirpath);
      } else {
        sprintf(fpath, "%s%s", dirpath, fixPath);
      }
    }
    ```
    Fungsi ```void changePath(char *fpath, const char *path, int isWriteOper, int isFileAsked) {``` akan melakukan pencarian terhadap _file name_ dengan nama __"encv1 _ "__. Kemudian, akan ada _pointer_ pada file tersebut dan ```int state = 0;``` state-nya menjadi __0__. Namun, ```if (ptr != NULL) {``` apabila _file name_ tidak bernama __"encv1 _ "__ dan ``` if (strstr(ptr+1, "/") != NULL) ``` apabila _file name_ bernama  __"encv1 _ "__ diikuti dengan _path_ yang berlanjut maka ```state = 1;``` state-nya menjadi __1__. <br>
    Tidak lupa untuk mendefinisikan __buffer__ yang merupakan tempat penyimpanan _processed path_ dari 3 (tiga) keperluan yang berbeda (__1.0__, __0.1__ dan __0.0__).
    ```bash
    char fixPath[1000];
    memset(fixPath, 0, sizeof(fixPath));
    ```
    Terdapat satu kondisi yang terdapat di dalam fungsi __fixPath__ yang akan bekerja untuk kondisi __1.0__ dan __0.1__ yang 
  akan mendefinisikan dan mengisi _buffer_.
    ```bash 
    if (ptr != NULL && state) {
    ptr = strstr(ptr+1, "/");
    char pathEncvDirBuff[1000];
    char pathEncryptedBuff[1000];
    strcpy(pathEncryptedBuff, ptr);
    strncpy(pathEncvDirBuff, path, ptr-path);
    ```
    Pada ```strncpy(pathEncvDirBuff, path, ptr-path);``` akan melakukan _copy_ dari _path_ ke __EncvDirBuff__. <br>
    ```bash
    if (isWriteOper) {
      char pathFileBuff[1000];
      char pathDirBuff[1000];
      getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
      decrypt(pathDirBuff, 0);
      sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
    } 
    ```
    Apabila hanya kondisi __1.0__ ```if (isWriteOper) {```  yang memiliki nilai, maka  ```char pathFileBuff[1000];``` akan mendefinisikan _file path_ dan ```char pathDirBuff[1000];``` akan mendefinisikan _directory path_ kemudian  menjalankan fungsi ```getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);``` yang bertujuan untuk mengembalikan _file path_ dan _directory path_ sesuai dengan _path_ yang telah tersimpan di dalam __pathEncryptedBuff__ dan dilanjutkan dengan proses ```decrypt(pathDirBuff, 0);``` pendeskripsian dan menyimpannya di dalam _buffer_ __fixPath__```sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);``` dengan format "__encv1 _ , directory path, file path__". <br>
    ```bash 
    else if (isFileAsked) {
      char pathFileBuff[1000];
      char pathDirBuff[1000];
      char pathExtBuff[1000];
      getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
      char *whereIsExtension = strrchr(pathFileBuff, '.');
      if (whereIsExtension-pathFileBuff <= 1) {
        decrypt(pathDirBuff, 0);
        decrypt(pathFileBuff, 0);
        sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
      } else {
        char pathJustFileBuff[1000];
        memset(pathJustFileBuff, 0, sizeof(pathJustFileBuff));
        strcpy(pathExtBuff, whereIsExtension);
        snprintf(pathJustFileBuff, whereIsExtension-pathFileBuff+1, "%s", pathFileBuff);
        decrypt(pathDirBuff, 0);
        decrypt(pathJustFileBuff, 0);
        sprintf(fixPath, "%s%s/%s%s", pathEncvDirBuff, pathDirBuff, pathJustFileBuff, pathExtBuff);
      }
      ```
      Untuk kondisi lainnya yakni apabila hanya kondisi __0.1__ ```else if (isFileAsked) {``` yang memiliki nilai, maka ```char pathFileBuff[1000];``` akan mendefinisikan _file path_, ```char pathDirBuff[1000];``` akan mendefinisikan _directory path_ serta ```char pathExtBuff[1000];``` akan mendefinisikan ekstensi dari file tersebut. Kemudian, _file path_ dan _directory path_ akan disimpan terlebih dahulu ke dalam __encrypted buffer__ menggunakan fungsi ```getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);```. Setelah melakukan penyimpanan _path_ ke dalam _buffer_, kita akan melakukan ```char *whereIsExtension = strrchr(pathFileBuff, '.');``` pencarian ekstensi dari file melalui _path_ yang telah ditentukan dengan melakukan pengecekan karakter ekstensi. Apabila ```if (whereIsExtension-pathFileBuff <= 1) {```tidak ditemukan ekstensi sesuai dengan _path_ yang ditentukan atau ditemukan ekstensi sesuai dengan _path_ yang ditentukan sejumlah 1 karakter, , maka akan dilakukan proses pendeskripsian ``` decrypt(pathDirBuff, 0);``` pada _directory path_ dan ```decrypt(pathFileBuff, 0);``` pada _file path_. Kemudian, hasil deskripsi tersebut disimpan ke dalam __fixPath__ ``` sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);```. dengan format "__encv1 _ , directory path, file path__".
Begitupun apabila ``` else {``` ditemukan ekstensi sesuai dengan _path_ yang ditentukan sejumlah lebih dari 1 karakter, maka akan dilakukan penyimpanan ekstensi dari file tersebut ke dalam _file path_ dan dilanjutkan dengan proses pendeskripsian ``` decrypt(pathDirBuff, 0);``` pada _directory path_ dan ```decrypt(pathFileBuff, 0);``` pada _file path_. Dan hasil deskripsi tersebut disimpan ke dalam __fixPath__ ```sprintf(fixPath, "%s%s/%s%s", pathEncvDirBuff, pathDirBuff, pathJustFileBuff, pathExtBuff);``` dengan format "__encv1 _ , directory path, file path, __ekstensi__".
      ```bash
        else {
        decrypt(pathEncryptedBuff, 0);
        sprintf(fixPath, "%s%s", pathEncvDirBuff, pathEncryptedBuff);
      }
      ```
      Fungsi di atas akan bekerja pada kondisi __0.0__ dimanan kondisi __1.0__ dan kondisi __0.1__ tidak memiliki nilai, maka ``` decrypt(pathEncryptedBuff, 0);``` akan terjadi proses pendeskripsian pada isi dari _buffer_ tersebut dan kemudian ``` sprintf(fixPath, "%s%s", pathEncvDirBuff, pathEncryptedBuff);``` disimpan di dalam __fixPath__ dengan format "__encv1 _ , decrypt path__".
      ```bash
          else {
        strcpy(fixPath, path);
      }
      ```
      
### Soal 4
Agar integritas _file system_ tersebut lebih terjamin, maka Jasir juga membuat _log file_ yang berguna untuk menyimpan daftar perintah dari _system call_ yang telah dijalankan. Adapun ketentuan dari _log file_ tersebut yakni sebagai berikut :
* _log_ akan terbagi menjadi 2 (dua) level yaitu level __INFO__ dan level __WARNING__.
* _log_ level __WARNING__ merupakan pencatatan log untuk _system call_ ```rmdir``` dan ```unlink```.
* Sehingga pencatatan log untuk _system call_ yang lain akan ditangani oleh _log_ level __INFO__.
* Adapun format _logging_ yakni ```[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]``` <br>
	* Keterangan : <br>
		LEVEL    : Level logging <br>
		yy   	 : Tahun dua digit <br>
		mm    	 : Bulan dua digit <br>
		dd    	 : Hari dua digit <br>
		HH    	 : Jam dua digit <br>
		MM    	 : Menit dua digit <br>
		SS    	 : Detik dua digit <br>
		CMD      : System call yang terpanggil <br>
		DESC     : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::) <br>
  #### Code :
  #### Penyelesaian :
  ```bash
  static const char *logpath = "/home/akmu/modul4/fuse/fs_log.txt";

  void logFile(char *level, char *cmd, int res, int lenDesc, const char *desc[]) {
  FILE *f = fopen(logpath, "a");
  time_t t;	
  struct tm *tmp;
  char timeBuff[100];

  time(&t);
  tmp = localtime(&t);
  strftime(timeBuff, sizeof(timeBuff), "%y%m%d-%H:%M:%S", tmp);

  fprintf(f, "%s::%s::%s::%d", level, timeBuff, cmd, res); 
  for (int i = 0; i < lenDesc; i++) {	
    fprintf(f, "::%s", desc[i]);
  }
  fprintf(f, "\n");

  fclose(f);
  ```
  * ```static const char *logpath = "/home/akmu/modul4/fuse/fs_log.txt";``` <br>
  _log file_ ini akan diletakkan pada _directory_ __/home/akmu/modul4/fuse__ dengan nama _log file_ tersebut adalah __fs _ log.txt__ 
  * Pada fungsi __logFile()__ ```void logFile(char *level, char *cmd, int res, int lenDesc, const char *desc[]) {``` akan mengatur format _logging_ yang sesuai dengan ketentuan _log file_ pada soal di atas antara lain ```level``` untuk menunjukkan _system call_ yang terjadi termasuk ke dalam level mana, ```cmd``` akan menunjukkan _system call_ yang terpanggil, ```res``` akan menyimopan status dari file tersebut, ```lenDesc``` akan menunjukkan panjang _file path_ dan ```desc[]``` akan menunjukkan _absolute file path_.
  * Kemudian, dilanjutkan dengan ```FILE *f = fopen(logpath, "a");``` proses membuat _log file_ sesuai dengan _directory path_ yang telah ditentukan. 
  * Dalam format _logging_ terdapat waktu pemanggilan _system call_ tersebut. Oleh karena itu, kita membutuhkan ``` char timeBuff[100];``` sebuah _buffer_ yang berguna untuk menyimpan waktu pemanggilan _system call_ tersebut dengan menggunakan fungsi ```strftime(timeBuff, sizeof(timeBuff), "%y%m%d-%H:%M:%S", tmp);```
  * Format _logging_ dalam fungsi __logFile()__ akan dituliskan ke dalam _log file_ menggunakan fungsi ```fprintf(f, "%s::%s::%s::%d", level, timeBuff, cmd, res); ```
  * Lalu, untuk menunjukkan ```desc[]``` _absolute file path_ maka akan digunakan fungsi iterasi di bawah ini 
    ```bash
    for (int i = 0; i < lenDesc; i++) {	
   	fprintf(f, "::%s", desc[i]);
    ```
    karena _absolute file path_ akan ditemukan sepanjang ```lenDesc``` yang menunjukkan panjang _file path_ tersebut.
    
__Note__ : Coding ini ketika kita jalankan __Error__ karena ada beberapa fungsi yang belum kami buat. Mohon Maaf :(

![Screenshot from 2020-05-02 10-03-27](https://user-images.githubusercontent.com/49342639/80853945-a3bb5880-8c5e-11ea-8516-0b3ea8e486f3.png)


![Screenshot from 2020-05-02 10-03-55](https://user-images.githubusercontent.com/49342639/80853989-e2e9a980-8c5e-11ea-838d-a5cd6bedb7eb.png)   
