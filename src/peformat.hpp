#pragma once
#include <fstream>
#include <string>
#include <list>
#include <memory>
#include "winheaders.hpp"

//!
//! \brief Класс работы с файлом в PE-формате
//!
class PEFormat
{
public:
    //! \brief Тип исполняемого файла
    enum ExecutableType
    {
        PE_TYPE_I386,
        PE_TYPE_AMD64
    };

    explicit PEFormat();
    ~PEFormat();

    //! \brief Загружает файл. Проверяет тип, формат и т.д.
    void load(const std::string& fname);

    //! \brief Возвращает список импортируемых библиотек
    std::list<std::string> importDlls();

    //! \brief Возвращает тип исполняемого файла
    ExecutableType type() const
    { return type_; }

private:
    //! \brief Конвертирует из чего-то во что-то
    std::uint32_t convertAddress(std::uint32_t addr, std::uint32_t align) const;

    //! \brief Адрес таблицы импорта
    uint32_t importVirtualAddr() const;

    //! \brief Значение выравния секций
    uint32_t sectionAligment() const;

    NTHeader nt;    //!< Структура PE заголовка
    std::ifstream pe_file;  //!< Поток чтения из файла
    std::unique_ptr<ImageSectionHeader[]> sections; //!< Секции
    ExecutableType type_;
};
