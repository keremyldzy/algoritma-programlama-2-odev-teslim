# Mars Kolonisi Yaşam Destek Simülatörü

Algoritma ve Programlama dersi kapsamında C dili kullanılarak geliştirilmiş metin tabanlı hayatta kalma simülasyonudur. Amaç, Mars'taki araştırma istasyonunda 15 gün boyunca kaynakları yöneterek hayatta kalmaktır.

## Komutlar

- `G` : Keşif görevi (Enerji ve oksijen harcar; su veya yedek parça bulma şansı sunar)
- `O` : Oksijen üretimi (Her 2 birim sudan 3 birim oksijen üretir)
- `T` : Tamir (Yedek parça kullanarak modül hasarını azaltır)
- `D` : Durum raporu (Mevcut kaynakları ve kritik uyarıları gösterir, tur harcamaz)
- `U` : Uyu / Dinlen (Enerji ve sağlık yeniler, oksijen tüketir)
- `K` : Kum fırtınası simülasyonu (`for` döngüsü ile fırtına hasarını hesaplar)
- `H` : Hava kilidi şifresi (`do-while` ile günlük ASCII şifresini çözüp kilidi mühürler)
- `Q` : Çıkış