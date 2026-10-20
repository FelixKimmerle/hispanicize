#include <cstdio>
#include <cstring>
#include <string>

static bool is_space(char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\v';
}

static bool is_terminator(char c)
{
    return c == '.' || c == '?' || c == '!';
}

// Characters that may precede the first word of a sentence, e.g. "(Really?)"
static bool is_opener(char c)
{
    return c == '"' || c == '\'' || c == '(' || c == '[';
}

// Characters that may follow the terminating punctuation, e.g. "Really?"
static bool is_closer(char c)
{
    return c == '"' || c == '\'' || c == ')' || c == ']';
}

std::string hispanicize(const std::string& input)
{
    std::string out;
    out.reserve(input.size() + input.size() / 8);

    const size_t n = input.size();
    size_t i = 0;

    while (i < n)
    {
        // Whitespace between sentences is copied verbatim
        while (i < n && is_space(input[i]))
        {
            out += input[i++];
        }
        if (i >= n)
        {
            break;
        }

        const size_t start = i;
        while (i < n && is_opener(input[i]))
        {
            i++;
        }
        const size_t body = i;

        // Find the run of terminators that ends this sentence. A run only counts
        // if it is followed by whitespace or the end of input, so "3.50" or
        // "example.com" don't split a sentence.
        size_t run_start = n, run_end = n;
        while (i < n)
        {
            if (!is_terminator(input[i]))
            {
                i++;
                continue;
            }
            size_t j = i;
            while (j < n && is_terminator(input[j]))
            {
                j++;
            }
            size_t k = j;
            while (k < n && is_closer(input[k]))
            {
                k++;
            }
            if (k == n || is_space(input[k]))
            {
                run_start = i;
                run_end = j;
                i = k;
                break;
            }
            i = j;
        }

        // Opening marks mirror the closing run: "?!" -> "¡¿"
        out.append(input, start, body - start);
        for (size_t r = run_end; r > run_start; r--)
        {
            if (input[r - 1] == '?')
            {
                out += "¿";
            }
            else if (input[r - 1] == '!')
            {
                out += "¡";
            }
        }
        out.append(input, body, i - body);
    }

    return out;
}

static std::string read_all(FILE* f)
{
    std::string data;
    char buf[1 << 16];
    size_t got;
    while ((got = std::fread(buf, 1, sizeof(buf), f)) > 0)
    {
        data.append(buf, got);
    }
    return data;
}

int main(int argc, char** argv)
{
    if (argc == 2 && (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0))
    {
        std::printf("Usage: %s [text...]\n"
                    "Prepends Spanish inverted question/exclamation marks to sentences.\n"
                    "Without arguments, the text is read from standard input.\n",
                    argv[0]);
        return 0;
    }

    if (argc == 1)
    {
        // Pipe mode: keep the input byte-for-byte, no extra newline
        const std::string out = hispanicize(read_all(stdin));
        std::fwrite(out.data(), 1, out.size(), stdout);
        return 0;
    }

    // Argument mode: join all arguments, so quoting is optional
    std::string text = argv[1];
    for (int a = 2; a < argc; a++)
    {
        text += ' ';
        text += argv[a];
    }
    const std::string out = hispanicize(text);
    std::fwrite(out.data(), 1, out.size(), stdout);
    std::fputc('\n', stdout);
    return 0;
}
