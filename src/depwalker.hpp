#pragma once
#include <string>
#include <set>
#include <cctype>

class DepWalker
{
public:
    explicit DepWalker();

    //!
    //! \brief Осуществялет поиск зависимостей
    //! \param fname Имя файла в PE формате
    //!
    //! Метод получает на вход имя PE файла, получает список
    //! испортируемых библиотек и, в зависимости от состояния
    //! флагов, копирует библиотеки в папку с PE файлов и их зависимости.
    //!
    void walk(const std::string& fname);

    //! \brief Устанавливает флаг рекурсивного поиска зависимостей
    void setRecursive(bool en)
    { recursive_ = en; }

    //! \brief Устанавливает флаг необходимости копирования библиотек
    void setCopy(bool en)
    { copy_ = en; }

    //!
    //! \brief Добавляет существующую директорию к путям поиска
    //! \param path Путь к директории
    //!
    void appendSearchPath(const std::string& path);

    //!
    //! \brief Устанавливает файл конфигурации
    //! \param fconfig Имя файла
    //!
    void setConfigurationFile(const std::string& fconfig);

protected:
    //!
    //! \brief Осуществляет поиск библиотеки.
    //! \param name Имя библиотеки
    //! \return std::string Полный путь к библиотеки.
    //! Пустая строка если библиотека не найдена.
    //!
    std::string findLibrary(const std::string& name);

private:
    bool recursive_;
    bool copy_;

    //! \brief Определяет операрот < (меньше) сравнения строк без учёта регистра
    struct LessString
    {
        bool operator()(const  std::string& lhs, const std::string& rhs)
        {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end(),
                                                [](char a, char b)
            { return std::tolower(a) < std::tolower(b); } );
        }
    };

    std::set<std::string> find_paths_;
    std::set<std::string, LessString> except_libs_;
};
