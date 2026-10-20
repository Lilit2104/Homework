#include <iostream>
#include <string>
#include <vector>
#include <memory>

class IPrintable {
    public:
        virtual void print() const = 0;
        virtual ~IPrintable() = default;
};

class ISavable {
    public:
        virtual void save(const std::string& path) const = 0;
        virtual ~ISavable() = default;
};

class ISerializable {
    public:
        virtual std::string serialize() const = 0;
        virtual ~ISerializable() = default;
};

class Document : public IPrintable, public ISavable, public ISerializable {
    private:
        std::string title;
        std::string content;

    public:
        Document(std::string title, std::string content)
            : title(std::move(title)), content(std::move(content)) {}

        void print() const override 
        {
            std::cout << "Document: " << title << "\n"
                  << content << "\n";
        }

        void save(const std::string& path) const override 
        {
            std::cout << "[Document] Պահպանվում է տեքստային ֆայլ՝ " << path
                  << " (" << content.size() << " բայթ)\n";
        }

        std::string serialize() const override 
        {
            return "{\"type\":\"Document\",\"title\":\"" + title +
                "\",\"content\":\"" + content + "\"}";
        }
};

class Image : public IPrintable, public ISavable, public ISerializable {
    private:
        std::string name;
        int width;
        int height;
        std::string format;

    public:
        Image(std::string name, int width, int height, std::string format)
            : name(std::move(name)), width(width), height(height),
            format(std::move(format)) {}

        void print() const override 
        {
            std::cout << "Image: " << name << "\n"
                  << "Չափ: " << width << "x" << height
                  << ", ֆորմատ: " << format << "\n"
                  << "[ պատկերի ASCII նախադիտում ]\n";
        }

        void save(const std::string& path) const override 
        {
            std::cout << "[Image] Պահպանվում է բինար " << format
                  << " ֆայլ՝ " << path << " ("
                  << (width * height * 3) << " բայթ, RGB)\n";
        }

        std::string serialize() const override 
        {
            return "{\"type\":\"Image\",\"name\":\"" + name +
               "\",\"width\":" + std::to_string(width) +
               ",\"height\":" + std::to_string(height) +
               ",\"format\":\"" + format + "\"}";
        }
};

void printAny(const IPrintable& p){ p.print(); }
void saveAny(const ISavable& s){ s.save("output.dat"); }
void serializeAny(const ISerializable& s) { std::cout << s.serialize() << "\n"; }

int main() {
   
    std::vector<std::unique_ptr<IPrintable>> printables;
    printables.push_back(std::make_unique<Document>("Report", "Բարև, աշխարհ!"));
    printables.push_back(std::make_unique<Image>("photo", 1920, 1080, "PNG"));

    std::cout << "IPrintable polymorphism\n";
    for (const auto& p : printables)
        p->print();

   
    std::cout << "\nMultiple inheritance\n";
    Document doc("Notes", "Կարևոր նշումներ");
    Image img("logo", 256, 256, "JPG");

    printAny(doc);  
    saveAny(doc);      
    serializeAny(doc);
    std::cout << "\n";
    printAny(img);      
    saveAny(img);      
    serializeAny(img);

   
    std::cout << "\nCross-casting\n";
    IPrintable* ip = &img;
    if (auto* is = dynamic_cast<ISavable*>(ip)) {
        is->save("converted.jpg");
    }

    return 0;
}