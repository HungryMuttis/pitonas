#include "runtime.h"

typedef std::wstring(*PitonFunc)(std::vector<std::wstring>);
typedef void (*RegisterCallback)(void* ctx, const wchar_t* name, PitonFunc func);
typedef void (*LoadFunc)(void*, RegisterCallback, const wchar_t*);

static void RuntimeRegisterHook(void* ctx, const wchar_t* name, PitonFunc func)
{
    Storage* storage = static_cast<Storage*>(ctx);
    storage->add(name, func);
}

void PitonRuntime::setInput(std::wistream& input)
{
	this->input = &input;
}
void PitonRuntime::setController(ConsoleController& controller)
{
	this->controller = controller;
}

void PitonRuntime::run(int argc, char* argv[])
{
    if (!this->input) return;

    Block block;
    std::vector<std::wstring> args;
    Storage functions;

    try
    {
        std::map<std::wstring, std::vector<std::wstring>> libOverrides;

        try
        {
            const char* nm = "libs";
            std::wstring content = ReadFileToWString(nm);
            std::wstringstream libs(content);

            std::wstring name = L"";
            std::vector<std::wstring> overrides;
            std::wstringstream currtext;
            bool open = false;
            wchar_t c;
            while (libs.get(c))
            {
                switch (c)
                {
                case L'"':
                    if (open)
                    {
                        open = false;
                        if (name.empty()) name = currtext.str();
                        else overrides.push_back(currtext.str());
                        currtext.str(L"");
                    }
                    else open = true;
                    break;
                case L';':
                    if (!open)
                    {
                        if (name.empty() || overrides.empty()) throw std::wstring(L"Invalid 'libs' file");
                        libOverrides[name] = overrides;
                        name = L"";
                        overrides.clear();
                        break;
                    }
                    [[fallthrough]];
                default:
                    if (open) currtext << c;
                    break;
                }
            }
            if (!name.empty()) throw std::wstring(L"Invalid 'libs' file");
        }
        catch (std::wstring) {}


        for (int i = 0; i < argc; i++)
            block.define(L"argv" + std::to_wstring(i));
        int line = 1;
        std::vector<std::pair<std::wstring, std::wstring>> libraries;
        block.build([this, &line]() -> wchar_t {
            wchar_t c;
            if (this->input->get(c))
            {
                if (c == L'\n') line++;
                return c;
            }
            return WEOF;
        }, [&line]() -> int { return line; }, &libraries);

        args.reserve(argc);
        for (int i = 0; i < argc; ++i)
        {
            std::string arg = argv[i];
            args.emplace_back(arg.begin(), arg.end());
        }

        for (const std::pair<std::wstring, std::wstring>& lib : libraries)
        {
            std::wstring dllName;
            std::wstring moduleName;

            size_t sp = lib.first.find(L'/');
            if (sp == std::wstring::npos)
            {
                dllName = lib.first;
                moduleName = lib.first;
            }
            else
            {
                dllName = lib.first.substr(0, sp);
                moduleName = lib.first.substr(sp + 1);
            }

            std::vector<std::wstring> files;
            std::map<std::wstring, std::vector<std::wstring>>::iterator iterator = libOverrides.find(dllName);
            if (iterator == libOverrides.end()) files.push_back(dllName);
            else files = iterator->second;

            HMODULE hDll = NULL;
            DWORD lerr = NULL;
            for (const std::wstring& file : files)
            {
                hDll = LoadLibraryW((file + L".dll").c_str());
                if (hDll) break;
                else lerr = GetLastError();
            }
            if (!hDll) throw std::wstring(L"Could not load library file '" + dllName + L".dll'. Error Code: " + std::to_wstring(lerr));

            LoadFunc entry = (LoadFunc)GetProcAddress(hDll, "Load");
            if (!entry) throw std::wstring(L"Failed to load library '" + dllName + L"'");

            Storage tmp;
            entry(&tmp, RuntimeRegisterHook, moduleName.c_str());
            std::wstring prefix = lib.second == lib.first ? moduleName : lib.second;
            if (prefix.find(L' ') != std::wstring::npos) throw std::wstring(L"Library name cannot contain spaces so it must be renamed");
            if (!lib.second.empty()) prefix += L'.';
            functions.merge(tmp, prefix);
        }

        block.prepare(functions);
    }
    catch (const std::runtime_error& error)
    {
        this->controller.showError(std::string(error.what()) + '\n');
        return;
    }
    catch (const std::wstring error)
    {
        this->controller.showError(L"[Prepare]: " + error + L'\n');
        return;
    }

    try
    {
        block.exec(args, nullptr);
    }
    catch (const std::wstring error)
    {
        this->controller.showError(L"[Runtime]: " + error + L'\n');
    }
}