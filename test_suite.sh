#!/bin/bash
# Akilli Kasa - Otomatik Test Paketi

set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}====================================================${NC}"
echo -e "${BLUE}         AKILLI KASA TEST PAKETI BASLATILIYOR       ${NC}"
echo -e "${BLUE}====================================================${NC}"

# 1. Programlarin derlenmesi
make -s clean
make -s all

# Test sayaclari
TOPLAM=0
BASARILI=0

calistir_test() {
    local test_adi="$1"
    local girdi="$2"
    local beklenen_kelime="$3"
    local beklenen_cikiskodu="$4"
    
    TOPLAM=$((TOPLAM + 1))
    echo -n "Test $TOPLAM: $test_adi ... "

    set +e
    cikti=$(printf "%b" "$girdi" | ./akilli_kasa 2>&1)
    kod=$?
    set -e

    if [ "$kod" -eq "$beklenen_cikiskodu" ] && echo "$cikti" | grep -q "$beklenen_kelime"; then
        echo -e "${GREEN}[BASARILI]${NC}"
        BASARILI=$((BASARILI + 1))
    else
        echo -e "${RED}[BASARISIZ]${NC}"
        echo "Beklenen kod: $beklenen_cikiskodu, Alinan kod: $kod"
        echo "Beklenen icerik: $beklenen_kelime"
        echo "Cikti:"
        echo "$cikti"
    fi
}

# Test 1: Odev ornegi (18.70 TL urun, 20.00 TL odenen)
calistir_test "Odev Ozel Ornegi (18.70 TL / 20.00 TL)" "18.70\n20.00\n" "TOPLAM VERILEN KUPUR SAYISI: 3 adet" 0

# Test 2: Tam odeme (Para ustu yok)
calistir_test "Tam Odeme (50.00 TL / 50.00 TL)" "50.00\n50.00\n" "Toplam Kupur Adedi: 0" 0

# Test 3: Buyuk banknot dagilimi (12.50 TL / 200.00 TL)
calistir_test "Buyuk Banknot Para Ustu (12.50 TL / 200.00 TL)" "12.50\n200.00\n" "TOPLAM VERILEN KUPUR SAYISI: 8 adet" 0

# Test 4: Tum kurus kademeleri (1.01 TL / 2.00 TL -> 0.99 TL)
calistir_test "Kurus Kademe Testi (1.01 TL / 2.00 TL)" "1.01\n2.00\n" "TOPLAM VERILEN KUPUR SAYISI: 8 adet" 0

# Test 5: Yetersiz odeme hatasi
calistir_test "Yetersiz Odeme Dogrulamasi (30.00 TL / 20.00 TL)" "30.00\n20.00\n" "HATA: Odenen para" 1

# Test 6: Negatif tutar hatasi
calistir_test "Negatif Tutar Dogrulamasi (-5.00 TL / 10.00 TL)" "-5.00\n10.00\n" "HATA: Tutarlar negatif olamaz" 1

echo -e "\n${BLUE}====================================================${NC}"
echo -e "Sonuc: ${GREEN}$BASARILI / $TOPLAM${NC} test basariyla tamamlandi."
echo -e "${BLUE}====================================================${NC}"

if [ "$BASARILI" -ne "$TOPLAM" ]; then
    exit 1
fi
