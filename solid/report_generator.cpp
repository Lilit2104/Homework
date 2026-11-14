#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

class ReportGenerator {
public:
    std::string generateReport() const {
        return "Monthly Report\nTotal: 15000\n";
    }
};

class IReportSaver {
public:
    virtual void save(const std::string& report,
                      const std::string& filename) const = 0;

    virtual ~IReportSaver() = default;
};

class TxtReportSaver : public IReportSaver {
public:
    void save(const std::string& report,
              const std::string& filename) const override {
        std::ofstream file(filename);

        if (!file)
            throw std::runtime_error("Cannot open file");

        file << report;
    }
};

class HtmlReportSaver : public IReportSaver {
public:
    void save(const std::string& report,
              const std::string& filename) const override {
        std::ofstream file(filename);

        if (!file)
            throw std::runtime_error("Cannot open file");

        file << "<html><body><pre>"
             << report
             << "</pre></body></html>";
    }
};

class CsvReportSaver : public IReportSaver {
public:
    void save(const std::string& report,
              const std::string& filename) const override {
        std::ofstream file(filename);

        if (!file)
            throw std::runtime_error("Cannot open file");

        file << "Report\n" << report;
    }
};

class ReportService {
private:
    const IReportSaver& saver;

public:
    explicit ReportService(const IReportSaver& saver)
        : saver(saver) {}

    void createAndSave(const ReportGenerator& generator,
                       const std::string& filename) const {
        std::string report = generator.generateReport();
        saver.save(report, filename);
    }
};

int main() {
    try {
        ReportGenerator generator;

        TxtReportSaver txtSaver;
        ReportService txtService(txtSaver);
        txtService.createAndSave(generator, "report.txt");

        HtmlReportSaver htmlSaver;
        ReportService htmlService(htmlSaver);
        htmlService.createAndSave(generator, "report.html");

        CsvReportSaver csvSaver;
        ReportService csvService(csvSaver);
        csvService.createAndSave(generator, "report.csv");
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}