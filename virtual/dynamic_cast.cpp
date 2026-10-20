#include <iostream>
#include <string>
#include <vector>
#include <memory>

class IPrintable {
    public:
        virtual void print() const = 0;
        virtual ~IPrintable() = default;
};

class ISerializable {
    public:
        virtual std::string serialize() const = 0;
        virtual ~ISerializable() = default;
};

class Document : public IPrintable, public ISerializable {
        std::string title;
    public:
        explicit Document(std::string title) : title(std::move(title)) {}

        void print() const override 
        {
            std::cout << "Document: " << title << "\n";
        }

        std::string serialize() const override 
        {
            return "{\"type\":\"Document\",\"title\":\"" + title + "\"}";
        }
};

class Image : public IPrintable {
        std::string name;
    public:
        explicit Image(std::string name) : name(std::move(name)) {}

        void print() const override
        {
            std::cout << "Image: " << name << "\n";
        }
};

class Video : public IPrintable {
        std::string name;
    public:
        explicit Video(std::string name) : name(std::move(name)) {}

        void print() const override 
        {
            std::cout << "Video: " << name << "\n";
        }
};

bool isSerializable(const IPrintable* ptr) 
{
    return dynamic_cast<const ISerializable*>(ptr) != nullptr;
}

void inspect(const IPrintable* ptr) 
{
    if (!ptr) {
        std::cout << "nullptr\n";
        return;
    }

    ptr->print();

    if (const auto* s = dynamic_cast<const ISerializable*>(ptr)) {
        std::cout << "  -> ISerializable է. Serialize: "
                  << s->serialize() << "\n";
    } else {
        std::cout << "  -> ISerializable ՉԻ\n";
    }
}

int main() {
    IPrintable* ptr;

    Document doc("Report");
    Image  img("photo.png");
    Video  vid("clip.mp4");

    ptr = &doc;
    std::cout << "isSerializable(doc) = " << std::boolalpha
              << isSerializable(ptr) << "\n";

    ptr = &img;
    std::cout << "isSerializable(img) = " << isSerializable(ptr) << "\n";

    ptr = &vid;
    std::cout << "isSerializable(vid) = " << isSerializable(ptr) << "\n";

    
    std::cout << "\nԲոլոր օբյեկտների ստուգում\n";
    std::vector<std::unique_ptr<IPrintable>> items;
    items.push_back(std::make_unique<Document>("Notes"));
    items.push_back(std::make_unique<Image>("logo.jpg"));
    items.push_back(std::make_unique<Video>("intro.mp4"));
    items.push_back(std::make_unique<Document>("Spec"));

    for (const auto& item : items)
        inspect(item.get());

    return 0;
}