# Bitirme-Tasarim-Projesi
Euler renk ve hareket yöntemlerini kullanarak geliştirdiğimiz videodan nabız algılama projemizin kaynak kodlarını ve kurulum için gerekli dosyaları bulabilirsiniz. [PDF](https://doneforaiur.github.io/Bitirme-Tasarim-Projesi/tez.pdf), [program](https://github.com/doneforaiur/Bitirme-Tasarim-Projesi/blob/main/Program/EVM/main.cpp), [arayüz](https://github.com/doneforaiur/Bitirme-Tasarim-Projesi/blob/main/Program/Aray%C3%BCz/EVM.cs).

## Kurulum
**DLL.zip** ve **EVM.zip** dosyalarını aynı klasöre çıkartınız. Linux kurulumu için **EVM.cpp** dosyasının derlenmesi ve OpenCV'nin kurulması gerekmektedir. Kullanıcı arayüzü Windows için geliştirilmiştir.

## Çalıştırma
Sağlanan **Arayüz.exe**'yi kullanarak kullanıcı arayüzünü başlatabilir veya komut satırından EVM.exe'yi direkt çalıştırabilirsiniz. Komut satırından **EVM.exe**'yi çalıştırmak istiyorsanız, aynı dosya içerisinde *config.txt* isimli dosya oluşturmanız gerekmektedir. Dosyanın içeriği;

| Parametre     | Tür                 | Örnek     |
| ------------- |---------------------| ----------|
| dosya_yolu    | `string`            | video.mp4 |
| alçak_frekans | `float < 1.0`       | 0.5       |
| yüksek_frekans| `float > 2.0`       | 2.5       |
| ala           | `float > 0.0`       | 75.5      |
| piramit_seviye| `int   > 1`         | 2         |
| gelişmiş      | `{true, false}`     | false     |
| kanal         | `{-1, 0, 1, 2}`     | 0         |

### "Gelişmiş" parametresi
Laplace piramit basamaklarını kullanarak hareketten kaynaklı renk değişimlerini en aza indirgemek içindir. Çıkış görüntüsünde bozulmalara yol açsa da bu beklenilen bir durumdur. Fourier dönüşümünde kullanılacak sinyaldeki gürültüyü azaltmak amaçlı kullanılır.

### "Kanal" parametresi
Renk kanalı sayısından feragat ederek performans artışı elde etmek içindir. `-1` ile bütün renk kanalları hesaplamalara dahil edilir. `0` mavi, `1` yeşil, `2` ise kırmızı kanal üstünde hesap yapılacağını belirtir. *Görüntü çıkışını siyah/beyaz yapar.*
