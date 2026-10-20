# hispanicize

Spanish tells you up front how to read a sentence: `¿` starts a question and `¡` starts an exclamation, so you know the tone before you get to the end. German and English only tell you at the very end.

`hispanicize` brings that to any text:

```
$ hispanicize "Wie spät ist es? Das ist unglaublich! Can you help me?"
¿Wie spät ist es? ¡Das ist unglaublich! ¿Can you help me?
```

## Build

You need a C++17 compiler (g++ or clang++) and `make`.

```bash
make release        # -> build/release/hispanicize
make debug          # -> build/debug/hispanicize (with -g -O0 -ftrapv)
make                # builds both
```

To install it system-wide (default prefix `/usr/local`):

```bash
sudo make install
```

`make uninstall` removes it again, and `PREFIX=~/.local make install` installs it for your user only.

## Usage

Pass the text as arguments, or pipe it in on stdin:

```bash
hispanicize "Is this real? Yes!"
hispanicize Is this real?            # arguments are joined with spaces
echo "Kommst du mit?" | hispanicize
hispanicize < input.txt > output.txt
```

When reading from stdin, all other characters pass through unchanged, including whitespace and line breaks. In argument mode a trailing newline is added.

### Rules

| Input                         | Output                          |
|-------------------------------|---------------------------------|
| `Wo bist du?`                 | `¿Wo bist du?`                  |
| `Wow!!!`                      | `¡¡¡Wow!!!`                     |
| `Really?!`                    | `¡¿Really?!` (marks are mirrored) |
| `"Really?" she asked.`        | `"¿Really?" she asked.`         |
| `It costs 3.50 euros!`        | `¡It costs 3.50 euros!`         |
| `No punctuation at the end`   | `No punctuation at the end`     |

- A sentence ends at a run of `.`, `?` or `!` followed by whitespace or the end of input (closing `"` `'` `)` `]` may come first). This means decimals and things like `example.com` don't split a sentence.
- The opening marks go after any leading `"` `'` `(` `[`.

### Limitations

- Abbreviations followed by a space (`z. B.`, `e.g. this`, `Dr. Smith`) still end a sentence.
- Spanish puts `¿` at the start of the *question clause* (`Si llueve, ¿vienes?`), not always at the start of the sentence. Finding that clause would need real parsing, so `hispanicize` always uses the start of the sentence.
- Only ASCII quotes and brackets count as openers and closers. Typographic quotes such as `„“` and `«»` are treated like any other character.

## Benchmark / memory check

`generate.py` writes about 10 MB of random German sentences to `german_text.txt`:

```bash
python3 generate.py
time ./build/release/hispanicize < german_text.txt > /dev/null
make test           # runs the release build on german_text.txt under valgrind
```
