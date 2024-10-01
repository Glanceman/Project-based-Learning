#include <imgui.h>
#include <ctype.h>
#include <stdio.h>

struct AppConsole
{
    AppConsole(bool enableTextInput);
    ~AppConsole();

    // Portable helpers
    /// Compare two string regardless the case sensitive without defined size
    /// if same return 0 else >0
    static int Stricmp(const char *s1, const char *s2)
    {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
        }
        return d;
    }
    /// Compare two string regardless the case sensitive with defined size
    /// if same return 0 else >0
    static int Strnicmp(const char *s1, const char *s2, int n)
    {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
            n--;
        }
        return d;
    }

    static char *Strdup(const char *s)
    {
        IM_ASSERT(s); // Assert that the input string is not null

        // Calculate the length of the string, including the null terminator
        size_t len = strlen(s) + 1;

        // Allocate memory for the new string using ImGui's memory allocator
        void *buf = ImGui::MemAlloc(len);
        IM_ASSERT(buf); // Assert that memory allocation was successful

        // Copy the original string to the newly allocated memory
        // and return the new string as a char pointer
        return (char *)memcpy(buf, (const void *)s, len);
    }

    static void Strtrim(char *s)
    {
        char *str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ') str_end--;
        *str_end = 0;
    }

    void ClearLog();

    void AddLog(const char *fmt, ...) IM_FMTARGS(2);

    void Draw(const char *title, bool *p_open);

    void ExecCommand(const char *command_line);

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData *data)
    {
        AppConsole *console = (AppConsole *)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData *data);

public:
    char                   InputBuf[256];
    ImVector<char *>       Items;
    ImVector<const char *> Commands;
    ImVector<char *>       History;
    int                    HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter        Filter;
    bool                   AutoScroll;
    bool                   ScrollToBottom;

private:
    bool _enableTextInput   = true;
    bool _enableDebugButton = false;
};

// static void ShowExampleAppConsole(bool *p_open)
// {
//     static ExampleAppConsole console;
//     console.Draw("Example: Console", p_open);
// }