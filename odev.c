#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Rastgele sayi uretici baslatma
    srand(time(NULL));

    // Durum degiskenleri
    int saglik = 100;
    int enerji = 100;
    int oksijen = 100;
    int su = 20;
    int yedekParca = 5;
    int modulHasari = 0;
    int kilitMuhurlu = 0;
    int gun = 1;
    int tur = 0;

    // Zorluk seviyesine gore degisen parametreler
    char zorluk;
    int gecerliZorluk = 0;
    int firtinaTurSayisi = 5;
    int firtinaTemelHasar = 8;
    int kesifEsikBonus = 0;

    printf("==================================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU (v1.0)\n");
    printf("   Hedef: Kurtarma ekibi gelene kadar 15 gun dayan!\n");
    printf("==================================================\n\n");

    // 1. ZORLUK SEVIYESI SECIMI (Gecersiz giriste tekrar ister)
    do {
        printf("Zorluk Seviyesi Secin [K: Kolay, N: Normal, Z: Zor]: ");
        scanf(" %c", &zorluk);

        switch (zorluk) {
            case 'k':
            case 'K':
                su = 30;
                yedekParca = 8;
                modulHasari = 0;
                firtinaTurSayisi = 3;
                firtinaTemelHasar = 5;
                kesifEsikBonus = 15;
                gecerliZorluk = 1;
                printf("\n[SISTEM] KOLAY zorluk secildi. Kaynaklar bol, firtinalar kisa.\n");
                break;

            case 'n':
            case 'N':
                su = 20;
                yedekParca = 5;
                modulHasari = 10;
                firtinaTurSayisi = 5;
                firtinaTemelHasar = 8;
                kesifEsikBonus = 0;
                gecerliZorluk = 1;
                printf("\n[SISTEM] NORMAL zorluk secildi. Standart Mars kosullari.\n");
                break;

            case 'z':
            case 'Z':
                su = 12;
                yedekParca = 3;
                modulHasari = 20;
                firtinaTurSayisi = 7;
                firtinaTemelHasar = 12;
                kesifEsikBonus = -10;
                gecerliZorluk = 1;
                printf("\n[SISTEM] ZOR zorluk secildi. Kaynaklar kisitli, firtinalar siddetli!\n");
                break;

            default:
                printf("[UYARI] Gecersiz secim! Lutfen K, N veya Z giriniz.\n");
                gecerliZorluk = 0;
                break;
        }
    } while (gecerliZorluk == 0);

    // Ana oyun dongusu degiskenleri
    char komut;
    int oyunDevam = 1;

    // 2. ANA KOMUT DINLEME DONGUSU (DO-WHILE)
    do {
        printf("\n--------------------------------------------------\n");
        printf("GUN: %d/15 | TUR: %d | Kilit Durumu: %s\n",
               gun, tur, (kilitMuhurlu == 1) ? "MUHURLU" : "ACIK");
        printf("Komutlar: [G]Kesif [O]Oksijen [T]Tamir [D]Durum [U]Uyu [K]Firtina [H]Hava Kilidi [Q]Cikis\n");
        printf("Komutunuz: ");
        scanf(" %c", &komut);

        int turHarcanacak = 0;

        switch (komut) {
            // --- G: KESIF GOREVI ---
            case 'g':
            case 'G': {
                if (enerji < 15 || oksijen < 10) {
                    printf("[IPTAL] Kesif icin yeterli enerji (min 15) veya oksijen (min 10) yok!\n");
                    break;
                }

                enerji -= 15;
                oksijen -= 10;
                turHarcanacak = 1;

                // Su/Kaynak bulma esigi mantiksal operatorlerle belirlenir
                int esik = ((enerji > 40 && oksijen > 30) ? 65 : 40) + kesifEsikBonus;
                if (esik > 90) esik = 90;
                if (esik < 15) esik = 15;

                int zar = rand() % 100;
                printf("[KESIF] Zar: %d / Esik: %d -> %s\n", zar, esik, (zar < esik) ? "BASARILI" : "BASARISIZ");

                // En az 3 farkli sonuc: Su bulma, Yedek parca bulma, Eli bos donme
                if (zar < esik / 2) {
                    int bulunanSu = 6 + (rand() % 5);
                    su += bulunanSu;
                    printf("-> Buzul damari bulundu! +%d birim su elde edildi.\n", bulunanSu);
                } else if (zar < esik) {
                    int bulunanParca = 2 + (rand() % 2);
                    yedekParca += bulunanParca;
                    printf("-> Eski uydu enkazi bulundu! +%d yedek parca toplandi.\n", bulunanParca);
                } else {
                    printf("-> Bolge tarandi ancak degerli bir kaynak bulunamadi (eli bos donuldu).\n");
                }

                // Yaralanma kontrolu (zar < 10 || modulHasari > 70)
                int kazaZari = rand() % 100;
                int kazaEsigi = 10;
                printf("[KAZA KONTROLU] Zar: %d / Esik: %d (veya Hasar > 70) -> %s\n",
                       kazaZari, kazaEsigi,
                       (kazaZari < kazaEsigi || modulHasari > 70) ? "YARALANMA" : "GUVENLI");

                if (kazaZari < kazaEsigi || modulHasari > 70) {
                    saglik -= 15;
                    printf("-> [TEHLIKE] Kesif sirasinda yaralandiniz! -15 Saglik.\n");
                }
                break;
            }

            // --- O: OKSIJEN URETIMI ---
            case 'o':
            case 'O': {
                if (enerji < 10) {
                    printf("[IPTAL] Elektroliz icin yeterli enerji (min 10) yok!\n");
                    break;
                }
                if (su < 2) {
                    printf("[IPTAL] Oksijen uretimi icin en az 2 birim su gereklidir!\n");
                    break;
                }
                if (oksijen >= 100) {
                    printf("[IPTAL] Oksijen tanklari zaten tamamen dolu (100)!\n");
                    break;
                }

                int harcanacakSu;
                printf("Mevcut Su: %d | Kullanilacak su miktarini girin (Her 2 su = 3 oksijen): ", su);
                scanf("%d", &harcanacakSu);

                if (harcanacakSu < 2 || harcanacakSu > su) {
                    printf("[IPTAL] Gecersiz su miktari! (En az 2, en fazla %d girebilirsiniz)\n", su);
                    break;
                }

                // Mod (%) operatoru ile tek/cift kontrolu
                int artikSu = harcanacakSu % 2;
                int islenenSu = harcanacakSu - artikSu;
                int uretilenOksijen = (islenenSu / 2) * 3;

                su -= islenenSu;
                enerji -= 10;
                oksijen += uretilenOksijen;
                turHarcanacak = 1;

                if (artikSu == 1) {
                    printf("[OKSIJEN] Tek sayi girildigi icin 1 birim su depoda artik olarak kaldi.\n");
                }
                printf("-> %d birim su islendi: +%d Oksijen uretildi, -10 Enerji harcandi.\n",
                       islenenSu, uretilenOksijen);
                break;
            }

            // --- T: TAMIR ---
            case 't':
            case 'T': {
                if (enerji < 10) {
                    printf("[IPTAL] Tamir icin yeterli enerji (min 10) yok!\n");
                    break;
                }
                if (yedekParca <= 0) {
                    printf("[IPTAL] Hic yedek parcaniz kalmadi! Tamir yapilamaz.\n");
                    break;
                }
                if (modulHasari == 0) {
                    printf("[IPTAL] Modulde hasar bulunmuyor (Hasar: 0).\n");
                    break;
                }

                // En az 3 kademeli IF - ELSE IF - ELSE zinciri
                if (modulHasari <= 30) {
                    // Hafif hasar: 1 parca, -15 hasar
                    yedekParca -= 1;
                    modulHasari -= 15;
                    enerji -= 10;
                    turHarcanacak = 1;
                    printf("[TAMIR - HAFIF HASAR] 1 yedek parca kullanildi. Hasar 15 birim azaltildi.\n");
                } else if (modulHasari <= 70) {
                    // Orta hasar: 2 parca (yoksa 1), -25 hasar
                    if (yedekParca >= 2) {
                        yedekParca -= 2;
                        modulHasari -= 25;
                        enerji -= 10;
                        turHarcanacak = 1;
                        printf("[TAMIR - ORTA HASAR] 2 yedek parca kullanildi. Hasar 25 birim azaltildi.\n");
                    } else {
                        printf("[IPTAL] Orta seviye hasar onarimi icin en az 2 yedek parca gerekli!\n");
                    }
                } else {
                    // Agir hasar (71-100): 3 parca, -40 hasar
                    if (yedekParca >= 3) {
                        yedekParca -= 3;
                        modulHasari -= 40;
                        enerji -= 10;
                        turHarcanacak = 1;
                        printf("[TAMIR - AGIR HASAR] 3 yedek parca kullanildi. Hasar 40 birim azaltildi.\n");
                    } else {
                        printf("[IPTAL] Agir hasar onarimi icin en az 3 yedek parca gerekli!\n");
                    }
                }
                break;
            }

            // --- D: DURUM RAPORU (Tur harcamaz) ---
            case 'd':
            case 'D': {
                printf("\n================ DURUM RAPORU ================\n");
                printf("Saglik       : %d / 100\n", saglik);
                printf("Enerji       : %d / 100\n", enerji);
                printf("Oksijen      : %d / 100\n", oksijen);
                printf("Su Miktari   : %d birim\n", su);
                printf("Yedek Parca  : %d adet\n", yedekParca);
                printf("Modul Hasari : %d / 100\n", modulHasari);
                printf("Hava Kilidi  : %s\n", (kilitMuhurlu == 1) ? "MUHURLU (1)" : "ACIK (0)");
                printf("----------------------------------------------\n");

                // Kritik uyarilar
                if (oksijen < 25) printf("[KRITIK UYARI] Oksijen seviyesi tehlikeli derecede dusuk!\n");
                if (saglik < 30)  printf("[KRITIK UYARI] Saglik durumu kritik! Dinlenmeniz gerekiyor.\n");
                if (enerji < 20)  printf("[KRITIK UYARI] Enerji tukenmek uzere!\n");
                if (modulHasari > 70) printf("[KRITIK UYARI] Modul hasari cok yuksek! Govde butunlugu riskte.\n");

                // Tek bir && kosulu ile tum sistemler normal kontrolu
                if (saglik >= 30 && enerji >= 20 && oksijen >= 25 && modulHasari <= 70) {
                    printf("[BILGI] TUM SISTEMLER NORMAL.\n");
                }
                printf("==============================================\n");
                turHarcanacak = 0; // Kural geregi D komutu tur harcamaz
                break;
            }

            // --- U: UYU / DINLEN ---
            case 'u':
            case 'U': {
                turHarcanacak = 1;
                enerji += 25;
                oksijen -= 8;

                // Ic ice IF ile oksijen kontrolu
                if (oksijen >= 20) {
                    saglik += 15;
                    printf("[DINLENME] Verimli uyku: +25 Enerji, +15 Saglik, -8 Oksijen.\n");
                } else {
                    if (oksijen < 20) {
                        saglik -= 10;
                        printf("[TEHLIKE - HIPOKSI] Oksijen 20'nin altindayken uyudunuz! +25 Enerji, -10 Saglik, -8 Oksijen.\n");
                    }
                }
                break;
            }

            // --- K: KUM FIRTINASI SIMULASYONU (FOR DONGUSU) ---
            case 'k':
            case 'K': {
                turHarcanacak = 1;
                int hasarAlinanTur = 0;
                int toplamSaglikKaybi = 0;
                int toplamOksijenKaybi = 0;

                printf("\n[ALARM] KUM FIRTINASI BASLADI! (Sure: %d firtina turu)\n", firtinaTurSayisi);

                for (int i = 1; i <= firtinaTurSayisi; i++) {
                    int firtinaZari = rand() % 100;
                    int firtinaEsigi = 60;
                    int carpan = (i % 2 == 0) ? 2 : 1; // Cift turlarda ruzgar siddetlenir (2x)

                    printf("  Firtina Turu %d %s | Zar: %d / Esik: %d -> ",
                           i, (carpan == 2) ? "[SIDDETLI RUZGAR 2X]" : "", firtinaZari, firtinaEsigi);

                    if (firtinaZari >= firtinaEsigi) {
                        printf("BASARISIZ (Korunuldu, hasar yok)\n");
                    } else {
                        printf("BASARILI (Firtina vurdu!)\n");
                        int olayTipi = firtinaZari % 3;

                        if (olayTipi == 0) {
                            // Oksijen sizintisi olayi
                            if (kilitMuhurlu == 1) {
                                printf("    -> Hava kilidi muhurlu oldugu icin oksijen sizintisi engellendi! (continue)\n");
                                continue;
                            }
                            int o2Kayip = firtinaTemelHasar * carpan;
                            oksijen -= o2Kayip;
                            toplamOksijenKaybi += o2Kayip;
                            hasarAlinanTur++;
                            printf("    -> Hava kilidi sizdirdi! -%d Oksijen.\n", o2Kayip);
                        } else if (olayTipi == 1) {
                            // Can kaybi
                            int canKayip = firtinaTemelHasar * carpan;
                            saglik -= canKayip;
                            toplamSaglikKaybi += canKayip;
                            hasarAlinanTur++;
                            printf("    -> Ucusan enkaz carpti! -%d Saglik.\n", canKayip);
                        } else {
                            // Modul hasari
                            int mHasar = firtinaTemelHasar * carpan;
                            modulHasari += mHasar;
                            hasarAlinanTur++;
                            printf("    -> Dis paneller darbe aldi! +%d Modul Hasari.\n", mHasar);
                        }
                    }

                    // Saglik veya oksijen 0'a duserse donguyu derhal sonlandir
                    if (saglik <= 0 || oksijen <= 0) {
                        printf("  [KRITIK] Yasam destek coktu! Firtina simulasyonu kesiliyor (break).\n");
                        break;
                    }
                }

                // Firtina sonu kilit muhru ruzgar asinmasiyla acilir
                kilitMuhurlu = 0;

                printf("--- FIRTINA OZETI ---\n");
                printf("Hasar Alinan Tur Sayisi : %d\n", hasarAlinanTur);
                printf("Toplam Kaybedilen Saglik: %d\n", toplamSaglikKaybi);
                printf("Toplam Kaybedilen O2    : %d\n", toplamOksijenKaybi);
                printf("---------------------\n");
                break;
            }

            // --- H: HAVA KILIDI SIFRESI (DO-WHILE DOGRULAMA DONGUSU) ---
            case 'h':
            case 'H': {
                if (kilitMuhurlu == 1) {
                    printf("[BILGI] Hava kilidi zaten muhurlu durumda!\n");
                    break;
                }
                if (enerji < 15) {
                    printf("[IPTAL] Guvenlik terminalini calistirmak icin en az 15 enerji gerekli!\n");
                    break;
                }

                turHarcanacak = 1;
                char dogruSifre = 'A' + (gun % 26);
                char girilenHarf;
                int kalanHak = 3;

                printf("\n[GUVENLIK TERMINALI] Hava kilidini muhurlemek icin sifreyi cozun.\n");
                printf("IPUCU: Sifre, Ingiliz alfabesinde 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

                // Hem sifrenin yanlis olmasini hem deneme hakkinin bitmemesini tek ifadede kontrol eder
                do {
                    printf("Tahmininiz (Kalan Hak: %d): ", kalanHak);
                    scanf(" %c", &girilenHarf);

                    // Kucuk harf girisini buyuk harfe cevirme (ASCII aritmetigi)
                    if (girilenHarf >= 'a' && girilenHarf <= 'z') {
                        girilenHarf = girilenHarf - ('a' - 'A');
                    }

                    if (girilenHarf != dogruSifre) {
                        kalanHak--;
                        enerji -= 5;
                        if (enerji < 0) enerji = 0;

                        if (girilenHarf < dogruSifre) {
                            printf("[HATALI] Sifre alfabede daha ILERIDE (ileri git). -5 Enerji.\n");
                        } else {
                            printf("[HATALI] Sifre alfabede daha GERIDE (geri git). -5 Enerji.\n");
                        }
                    }
                } while (girilenHarf != dogruSifre && kalanHak > 0);

                if (girilenHarf == dogruSifre) {
                    kilitMuhurlu = 1;
                    printf("[BASARILI] Dogru sifre (%c)! Hava kilidi muhurlendi.\n", dogruSifre);
                } else {
                    modulHasari += 20;
                    printf("[BASARISIZ] 3 deneme hakki doldu! Guvenlik kilitlendi ve zorlama sonucu +20 Modul Hasari olustu.\n");
                }
                break;
            }

            // --- Q: CIKIS ---
            case 'q':
            case 'Q': {
                printf("\n[SISTEM] Simülasyon kullanici istegiyle sonlandiriliyor...\n");
                oyunDevam = 0;
                break;
            }

            // --- TANIMSIZ KOMUT ---
            default: {
                printf("[UYARI] Tanimsiz komut ('%c')! Lutfen gecerli bir harf girin. (Tur harcanmadi)\n", komut);
                turHarcanacak = 0;
                break;
            }
        }

        // --- SINIRLANDIRMA KONTROLLERI (0 - 100 ARASI) ---
        if (saglik < 0) saglik = 0;
        if (saglik > 100) saglik = 100;

        if (enerji < 0) enerji = 0;
        if (enerji > 100) enerji = 100;

        if (oksijen < 0) oksijen = 0;
        if (oksijen > 100) oksijen = 100;

        if (modulHasari < 0) modulHasari = 0;
        if (modulHasari > 100) modulHasari = 100;

        // --- ZAMAN AKISI VE GUN GECISI ---
        if (oyunDevam == 1 && turHarcanacak == 1) {
            tur++;

            // Her 3 turda bir yeni gun baslar
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                printf("\n>>> YENI GUN BASLADI! (Gun %d) | Gunluk tuketim: -8 Oksijen.\n", gun);

                if (modulHasari > 70) {
                    saglik -= 5;
                    printf(">>> [KRITIK HASAR CEZASI] Modul hasari 70'in uzerinde oldugu icin -5 Saglik kaybettiniz!\n");
                }

                // Gun gecisi sonrasi sinir kontrolu
                if (oksijen < 0) oksijen = 0;
                if (saglik < 0) saglik = 0;
            }

            // --- OYUN SONU KOSULLARI ---
            if (saglik <= 0 || oksijen <= 0) {
                printf("\n==================================================\n");
                printf("   [OYUN BITTI - KAYBETTINIZ]\n");
                if (saglik <= 0) printf("   Sebep: Sagliginiz tukendi.\n");
                if (oksijen <= 0) printf("   Sebep: Oksijeniniz tukendi.\n");
                printf("==================================================\n");
                oyunDevam = 0;
            } else if (gun > 15) {
                // 15. gun tamamlandiginda (16. gune gecildiginde) kurtarma gelir
                gun = 15;
                printf("\n==================================================\n");
                printf("   [TEBRIKLER - KAZANDINIZ!]\n");
                printf("   15 gun boyunca hayatta kaldiniz ve kurtarma ekibi ulasti!\n");
                printf("==================================================\n");
                oyunDevam = 0;
            }
        }

    } while (oyunDevam == 1);

    // --- FINAL SONUC RAPORU VE PUAN HESAPLAMA ---
    int puan = (gun * 100) + (saglik * 2) + (oksijen * 2) + (su * 5) + (yedekParca * 10) - (modulHasari * 2);
    if (puan < 0) puan = 0;

    printf("\n================ SIMULASYON SONUC RAPORU ================\n");
    printf("Hayatta Kalinan Gun : %d\n", gun);
    printf("Toplam Gecen Tur    : %d\n", tur);
    printf("Son Saglik          : %d\n", saglik);
    printf("Son Enerji          : %d\n", enerji);
    printf("Son Oksijen         : %d\n", oksijen);
    printf("Kalan Su            : %d birim\n", su);
    printf("Kalan Yedek Parca   : %d adet\n", yedekParca);
    printf("Son Modul Hasari    : %d\n", modulHasari);
    printf("---------------------------------------------------------\n");
    printf("TOPLAM BASARI PUANI : %d\n", puan);
    printf("=========================================================\n");

    return 0;
}