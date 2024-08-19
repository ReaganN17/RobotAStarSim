//header
union EventByte{
	unsigned char byte = 0;

	struct {
		bool cond1 : 1;
		bool cond2 : 1;
		bool cond3 : 1;
		bool pointer : 1;
		bool restart : 1;
		bool running : 1;
		bool instance : 1;
		bool interupt : 1;
	};
};
struct BasicEvent {
	BasicEvent** pointer;
	//god strike me down
	EventByte eb;
	
	BasicEvent();
	BasicEvent(BasicEvent**);
	virtual ~BasicEvent();

	virtual void start();
	virtual void init();
	virtual void loop();
	virtual void end();
	virtual void endNtransfer();
	virtual void endNstart();
	virtual void interrupt();
};

//event handler
vector<BasicEvent*> events;

void runEvents() {
	for (auto i : events) {
		(i)->loop();
	}
}

void endEvents() {
	for (auto i : events) {
		(i)->interrupt();
	}
}

void pauseEvents() {
	for (auto i : events) {
		(i)->eb.running = false;
	}
}

void unpauseEvents() {
	for (auto i : events) {
		(i)->eb.running = true;
	}
}

//cpp file

BasicEvent::BasicEvent() {}
BasicEvent::BasicEvent(BasicEvent** pointer) : pointer(pointer) { eb.pointer = true; };
BasicEvent::~BasicEvent() {};

void BasicEvent::start() {
	if (!eb.instance) {
		events.push_back(&*this);
		eb.running = true;
		eb.instance = true;
		eb.interupt = false;
		init();
	}
	else if (eb.restart) {
		interrupt();
		events.push_back(&*this);
		eb.running = true;
		eb.instance = true;
		eb.interupt = false;
		init();
	}
}

void BasicEvent::init() {
	
}

void BasicEvent::loop() {
	if (!eb.running) return;
	if (eb.interupt) { interrupt(); return; }
}

void BasicEvent::end() {
	events.erase(find(events.begin(), events.end(), &*this));
	eb.instance = false;
	eb.running = false;

	if (eb.pointer) { *pointer = nullptr; }

	BasicEvent::~BasicEvent();

}

void BasicEvent::endNstart() {
	events.erase(find(events.begin(), events.end(), &*this));
	eb.instance = false;
	eb.running = false;

	start();
}

void BasicEvent::endNtransfer() {
	events.erase(find(events.begin(), events.end(), &*this));
	eb.instance = false;
	eb.running = false;

	BasicEvent::~BasicEvent();
}

void BasicEvent::interrupt() {
	
	end();
}

