# # Arduino Game Pad

## Popis projektu

Tento projekt představuje jednoduchý herní ovladač (game pad) vytvořený pomocí platformy Arduino. Ovladač je navržen tak, aby umožňoval hraní klasických her jako Tetris, Pong a dalších. Projekt je zaměřen na demonstraci základních elektronických a programovacích dovedností s Arduinem, včetně využití displeje, tlačítek a dalších vstupních a výstupních komponent.

### Klíčové vlastnosti:
- **Plně funkční game pad**: Vytvořený z Arduina s tlačítky pro ovládání her.
- **Integrovaný displej**: Pro zobrazování herního prostředí.
- **Podpora více her**: Tetris, Pong a možnost přidávání dalších her.
- **Ovládací prvky**: Tlačítka pro pohyb, potvrzení a reset.

## Hardware

Pro projekt je použit následující hardware:
- Arduino (Uno, Nano nebo jiný kompatibilní model)
- OLED nebo LCD displej pro zobrazování her
- Tlačítka pro ovládání (minimálně 4 směrová tlačítka + potvrzovací tlačítko)
- Rezistory pro tlačítka (pokud jsou potřeba)
- Napájení pro Arduino (baterie nebo USB)
- Spojovací vodiče a deska (breadboard)

## Software

Projekt využívá Arduino IDE pro programování logiky her a ovládání game padu. Každá hra je implementována jako samostatná část kódu, kterou lze snadno přepínat nebo rozšiřovat.

### Implementované hry:
1. **Tetris**: Klasická hra s padajícími bloky, které je třeba správně umístit a vytvořit plné řádky.
2. **Pong**: Dvourozměrná hra s míčkem a dvěma pálkami, cílem je zabránit míčku vypadnout z hracího pole.

## Instalace a spuštění

1. Stáhněte nebo naklonujte tento repozitář.
2. Otevřete projekt v Arduino IDE.
3. Nahrajte kód na Arduino desku.
4. Připojte displej a tlačítka podle schématu zapojení.
5. Zapněte Arduino a vyberte hru, kterou chcete hrát.

## Schéma zapojení

**Schéma zapojení** (doplnit obrázek nebo odkaz) zahrnuje propojení displeje a tlačítek s Arduino deskou. Ujistěte se, že správně propojujete piny podle kódu.

## Možné vylepšení

V budoucnu je možné projekt dále rozšířit a vylepšit následujícími způsoby:

- **Přidání více her**: Například Snake, Arkanoid, Space Invaders nebo jiné retro hry.
- **Zvukové efekty**: Přidání zvukového výstupu (buzzer) pro obohacení herního zážitku.
- **Bezdrátové ovládání**: Implementace bezdrátové komunikace (např. pomocí Bluetooth modulu) pro možnost hrát hry na vzdálenost.
- **Vícebarevný displej**: Výměna displeje za vícebarevný pro lepší grafiku her.
- **Vylepšení ovládacích prvků**: Přidání joysticku nebo více tlačítek pro komplexnější hry.
- **Ukládání skóre**: Možnost ukládání a zobrazování nejvyšších skóre pomocí externí paměti.
- **Nabíjecí baterie**: Implementace nabíjecí baterie pro větší mobilitu zařízení.

## Závěr

Tento projekt ukazuje, jak lze s pomocí Arduina vytvořit plně funkční game pad a hrát jednoduché retro hry. Je to skvělý příklad kombinace hardwarového a softwarového vývoje s možností dalšího rozšíření a vylepšení.

## Autor

Projekt byl vytvořen jako maturitní práce na [tvoje škola] v roce [rok].

