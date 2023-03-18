void listAll(string path, const File* f) {
    if (f == nullptr) return;
    if(f->name() == path) cout << path << "/" << endl;
    else if(dynamic_cast<const PlainFile*>(f) != nullptr) {
        cout << path << "/" << f->name() << endl;
    }
    else cout << path << "/" << f->name() << "/" << endl;
    if (f->files() != nullptr) {
        for (const File* file : *(f->files())) {
            if(f->name() == path) listAll(path, file);
            else listAll(path + "/" + f->name(), file);
        }
    }
}
