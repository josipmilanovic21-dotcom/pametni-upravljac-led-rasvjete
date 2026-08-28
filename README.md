# Pametni upravljač LED rasvjete

Seminarski projekt iz kolegija **Ugrađeni sustavi**.

Projekt prikazuje jednostavan ugrađeni sustav za automatsku regulaciju LED rasvjete pomoću **Arduino Nano** mikrokontrolera, **LDR senzora**, **potenciometra**, **LED diode** i **LCD zaslona**.

---

## Opis projekta

Cilj projekta je napraviti sustav koji automatski regulira jačinu LED rasvjete ovisno o količini svjetla u okolini.

Sustav radi na sljedeći način:

1. **LDR senzor** mjeri trenutnu razinu ambijentalnog svjetla.
2. **Potenciometar** služi za zadavanje referentne vrijednosti, odnosno željene razine svjetla.
3. **Arduino Nano** uspoređuje izmjerenu vrijednost s referentnom vrijednosti.
4. Ako je prostor tamniji od zadane vrijednosti, LED dioda svijetli jače.
5. Ako ima dovoljno svjetla, LED dioda se smanjuje ili gasi.
6. Vrijednosti se prikazuju u **Serial Monitoru**, a u Wokwi simulaciji i na **LCD 16x2 I2C zaslonu**.

Ovaj projekt predstavlja jednostavan primjer sustava s **povratnom vezom**, jer Arduino stalno mjeri trenutno stanje i prema tome prilagođava izlaz.

---

## Korištene komponente

- Arduino Nano
- Breadboard
- LDR senzor / fotootpornik
- Potenciometar 10 kΩ
- LED dioda
- Otpornik 220 Ω za LED diodu
- Otpornik 10 kΩ za LDR djelitelj napona
- LCD 16x2 I2C zaslon
- Spojne žice

---

## Spojevi

### Arduino Nano pinovi

| Komponenta | Arduino Nano pin | Objašnjenje |
|---|---:|---|
| LDR senzor | A0 | Mjerenje stvarne razine svjetla |
| Potenciometar | A1 | Zadavanje referentne vrijednosti |
| LED dioda | D9 | PWM izlaz za regulaciju jačine LED diode |
| LCD SDA | A4 | I2C podatkovna linija |
| LCD SCL | A5 | I2C clock linija |
| Napajanje | 5V | Plus šina na breadboardu |
| Masa | GND | Minus šina na breadboardu |

---

## Spajanje potenciometra

Potenciometar ima tri pina:

```text
jedan krajnji pin  -> 5V
srednji pin        -> A1
drugi krajnji pin  -> GND
```

Potenciometar radi kao djelitelj napona. Okretanjem potenciometra mijenja se napon na srednjem pinu. Arduino taj napon čita na analognom pinu A1 kao vrijednost od 0 do 1023.

---

## Spajanje LDR senzora

LDR je spojen s otpornikom 10 kΩ kao djelitelj napona:

```text
5V -> LDR -> A0 -> otpornik 10 kΩ -> GND
```

Točka između LDR-a i otpornika spojena je na A0.

Arduino ne mjeri otpor direktno, nego mjeri napon. Kako se količina svjetla mijenja, mijenja se otpor LDR-a, a time se mijenja i napon koji Arduino čita na A0.

---

## Spajanje LED diode

LED dioda je spojena preko otpornika 220 Ω:

```text
D9 -> otpornik 220 Ω -> duža noga LED diode
kraća noga LED diode -> GND
```

Otpornik je potreban jer ograničava struju kroz LED diodu i štiti LED diodu i Arduino pin.

Pin D9 koristi se zato što je to PWM pin. PWM omogućuje regulaciju jačine LED diode, a ne samo uključivanje i isključivanje.

---

## Spajanje LCD zaslona u Wokwi simulaciji

LCD 16x2 koristi I2C komunikaciju.

```text
LCD VCC -> 5V
LCD GND -> GND
LCD SDA -> A4
LCD SCL -> A5
```

Kod Arduino Nano pločice:

```text
A4 = SDA
A5 = SCL
```

U fizičkoj izvedbi korišteni LCD modul nije imao zalemljene pinove za I2C priključak, pa prikaz nije bio pouzdan. Zbog toga je regulacijski dio projekta testiran fizički, a LCD/I2C dio dodatno je prikazan u Wokwi simulaciji.

---

## Objašnjenje rada programa

Arduino u glavnoj petlji stalno čita dvije analogne vrijednosti:

```cpp
int ldrValue = analogRead(ldrPin);
int refValue = analogRead(potPin);
```

- `ldrValue` predstavlja stvarnu izmjerenu razinu svjetla.
- `refValue` predstavlja željenu razinu svjetla koju korisnik zadaje potenciometrom.

Zatim se računa razlika:

```cpp
int error = refValue - ldrValue;
```

Ako je `error` pozitivan, znači da je izmjerena razina svjetla manja od željene i LED se pojačava:

```cpp
ledValue = map(error, 0, 1023, 0, 255);
```

Vrijednosti analognih ulaza su u rasponu od 0 do 1023, dok PWM izlaz koristi raspon od 0 do 255. Zato se koristi funkcija `map()`.

Ako je `error` manji ili jednak nuli, znači da ima dovoljno svjetla i LED se gasi:

```cpp
ledValue = 0;
```

Na kraju se vrijednost šalje na LED diodu:

```cpp
analogWrite(ledPin, ledValue);
```

---

## PWM objašnjenje

PWM znači **Pulse Width Modulation**.

Arduino vrlo brzo pali i gasi LED diodu. Ako je LED uključena veći dio vremena, ljudskom oku izgleda kao da svijetli jače. Ako je uključena manji dio vremena, izgleda kao da svijetli slabije.

PWM vrijednosti:

```text
0   = LED ugašena
255 = LED maksimalno svijetli
```

---

## Testiranje

Tijekom izrade napravljeno je nekoliko testova.

### 1. Test LED diode

Datoteka:

```text
test_led_dioda.ino
```

Ovaj test pali i gasi LED svake sekunde. Time se provjerava rade li LED, otpornik, GND spoj i pin D9.

### 2. Test potenciometra i LED diode

Datoteka:

```text
pot_led_test.ino
```

Ovaj test čita vrijednost potenciometra i prema njoj regulira jačinu LED diode. Time se provjerava radi li analogni ulaz A1 i PWM izlaz D9.

### 3. Test LDR senzora

Datoteka:

```text
ldr_test.ino
```

Ovaj test ispisuje vrijednost LDR senzora u Serial Monitor. Kada se LDR pokrije ili osvijetli, vrijednosti se mijenjaju.

### 4. Test cijelog sustava bez LCD-a

Datoteka:

```text
ldr_pot_upravljac_rasvjete.ino
```

Ovaj kod povezuje LDR, potenciometar i LED diodu. To je fizički testiran dio projekta na breadboardu.

### 5. I2C scanner

Datoteka:

```text
test_I2C_health.ino
```

Ovaj test traži I2C uređaje i ispisuje pronađenu adresu u Serial Monitor. Koristi se za provjeru vidi li Arduino LCD modul.

---

## Datoteke u projektu

| Datoteka | Opis |
|---|---|
| `upravljac_rasvjete.ino` | Glavni program s LCD prikazom |
| `ldr_pot_upravljac_rasvjete.ino` | Glavni fizički test bez LCD-a |
| `test_led_dioda.ino` | Test LED diode |
| `pot_led_test.ino` | Test potenciometra i LED diode |
| `ldr_test.ino` | Test LDR senzora |
| `test_I2C_health.ino` | I2C scanner za LCD modul |

---

## Wokwi simulacija

Projekt je dodatno napravljen u Wokwi simulaciji kako bi se prikazao i LCD dio sustava.

U Wokwi simulaciji korišteni su:

- Arduino Nano
- LDR senzor
- Potenciometar
- LED dioda
- Otpornik 220 Ω
- LCD 16x2 I2C zaslon

Wokwi simulacija omogućuje promjenu vrijednosti potenciometra i LDR senzora. LED dioda i LCD prikaz reagiraju na promjene jednako kao što bi sustav trebao raditi u stvarnoj izvedbi.

---

## Zaključak

Projekt pokazuje osnovni princip automatske regulacije rasvjete. Arduino Nano očitava stvarnu razinu svjetla pomoću LDR senzora i uspoređuje je s referentnom vrijednosti koju korisnik zadaje potenciometrom. Na temelju razlike Arduino preko PWM signala regulira LED diodu.

Fizički dio sustava s LDR senzorom, potenciometrom i LED diodom uspješno je testiran na breadboardu. LCD prikaz i I2C komunikacija demonstrirani su u Wokwi simulaciji.
