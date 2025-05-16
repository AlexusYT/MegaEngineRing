# Управление ресурсами

\note Руководство не завершено. Описанное здесь может несоответствовать действительности.

\tableofcontents

## Концепт

Система загрузки ресурсов работает по принципу запрос-ответ. Сначала создаются запросы на загрузку всех необходимых
ресурсов, а затем, в зависимости от политик загрузки, происходит исполнение запросов, используя указанный в запросе
загрузчик. После этого результаты исполнения записываются в отдельный класс, с помощью которого можно их получить.

Существует две политики загрузки ресурсов:

1. Обычная загрузка --- загрузка во время подготовки сцены.
2. Ленивая загрузка --- загрузка в момент получения ресурса.

Ресурс, загруженный ленивым запросом, называется ленивым ресурсом, тогда как загруженный обычным запросом - обычным.

Для выбора необходимой политики загрузки, нужно определить, насколько часто ресурс будет использоваться:

Например, модель плюшевого мишки, который находится в отдаленной комнате, имеет смысл загружать лениво,
так как высока вероятность, что игрок за игровую сессию не доберется до него. Это сэкономит время загрузки сцены и
ресурсы компьютера.

Обычную загрузку стоит использовать, если ресурс будет использоваться повсеместно, например, шейдер отображения.

Факторы, такие как размер и сложность загрузки ресурса не должны влиять на выбор политики. Тяжелый ресурс при ленивой
загрузке лишь немного отложит появление объекта, который его использует, как и обычная загрузка отложит момент
отображения сцены. Кроме того, если есть необходимость в ленивой загрузки "тяжелой" модели, стоит провести ее
оптимизацию. Если же после оптимизации модель все равно долго загружается, рекомендуется рассмотреть вариант применения
[LOD](https://ru.wikipedia.org/wiki/Level_of_Detail) (Level of detail, уровни детализации).
\todo Создать ссылку на руководство по LOD

\note В текущей версии ресурсы загружаются в потоке рендера. В будущих обновлениях
загрузка будет выполняться в отдельном потоке.

\todo Описать возможные ошибки и исключения при работе алгоритма загрузки, если таковые имеются.

## Запросы на загрузку

### Класс запроса

В зависимости от политики загрузки, класс запроса должен наследовать либо `mer::sdk::RegularRequest` для обычного
запроса, либо `mer::sdk::LazyRequest` для ленивого. Класс запроса должен переопределять следующие методы:

- `mer::sdk::ResourceRequest::getName()` -- метод должен возвращать название ресурса, которое будет использоваться
  в
  дальнейшем для получения результата загрузки. Если запрос универсальный, то название можно передавать через параметр
  конструктора класса.
- `mer::sdk::ResourceRequest::getLoader()` -- метод должен возвращать экземпляр класса, производного от
  `mer::sdk::ResourceLoader`. Создание и хранение экземпляра этого класса в классе запроса настоятельно не
  рекомендуется, так как данный метод может ни разу не вызваться. Такая ситуация может произойти в случае, если запрос
  не был исполнен и он был "ленивым". Это приведет к повышенному расходу памяти.

\todo Описать принцип создания зависимостей.

### Пример реализации класса запроса

```c++
class MyUniversalRequest : public mer::sdk::RegularRequest {
	std::string name;
	
	bool foo; //Custom data
public:
	explicit MyUniversalRequest(const std::string &pName, bool pFoo) : name(pName), foo(pFoo) {}
	
	[[nodiscard]] std::shared_ptr<mer::sdk::ResourceLoader> getLoader() const override{
		return std::make_shared<MyUniversalLoader>();
	}

	[[nodiscard]] std::string getName() const override { return name; }

	
	[[nodiscard]] bool isFoo() const { return foo; }
};
```

### Класс загрузчика

Класс загрузчика ресурса --- это производный класс от `mer::sdk::ResourceLoader`. Необходимо переопределить
единственный метод класса --  `mer::sdk::ResourceLoader::load`.

Метод должен возвращать экземпляр класса, производного от `mer::sdk::IResource` или `nullptr` в случае ошибки.
Параметры метода:

1. `const std::shared_ptr<ResourceRequest> &pRequest` --- указатель на запрос. Его можно использовать для получения
   информации, необходимой для загрузки ресурса.
2. `const sdk::ReportMessagePtr &pError,` --- указатель на структуру, содержащую сообщение об ошибки. Этот
   параметр необходимо использовать в случае возникновения ошибки, т. е. при возврате `nullptr`.
3. `const std::shared_ptr<Resources> &pDependencies` --- ресурсы, которые были запрошены как зависимости. Если
   зависимости не были указаны в запросе, то класс не содержит ни одного ресурса.

\warning Ответственность по обработке ошибок и исключительных ситуаций во время загрузки ресурса загрузчиком возлагается
на разработчика загрузчика.

\note Следует учесть, что возврат `nullptr` из метода загрузки, будет интерпретироваться движком как ошибка загрузки.
Это приведет к том, что запрос не будет исполнен и ресурс не будет добавлен в список ресурсов сцены. Из такой ситуации
существует два выхода:

\note 1. При возникновении ошибки загрузки ресурса, вернуть не `nullptr`, а экземпляр класса загружаемого ресурса с
резервным содержимым и сообщить об ошибке в консоль, используя метод `mer::sdk::Logger::error` с аргументом
ошибки (`mer::sdk::ReportMessagePtr`).

\note 2. Вернуть `nullptr` и перед запросом ресурса проверять его наличие в списке ресурсов. Дальнейшие действия зависят
от целей и задач разработчика. Можно попробовать получить резервный ресурс и если он также отсутствует, сообщить об этом
игроку фатальной ошибкой. \todo Предоставить пользователю API для отображения ошибок в интерфейсе.

\note Независимо от того, какой вариант решения будет выбран, в конечном продукте подобных ошибок быть не должно, так
как это может привести к отрицательному игровому опыту. Эти способы полезны только на этапе разработки и тестирования.

В зависимости от политики загрузки, метод `mer::sdk::ResourceLoader::load` вызывается в разные моменты работы
движка:

- Если запрос является [обычным](#mer::sdk::RegularRequest), то метод вызывается во время загрузки сцены.
- Если запрос [ленивый](#mer::sdk::LazyRequest), то метод вызывается только при первом получении ресурса.

\note Ленивый запрос может быть никогда не исполнен, если ресурс ни разу не был получен.

### Пример реализации класса загрузчика

```c++
class MyUniversalLoader : public mer::sdk::ResourceLoader {
public:
	std::shared_ptr<mer::sdk::IResource> load(const std::shared_ptr<mer::sdk::ResourceRequest> &pRequest,
									const mer::sdk::ReportMessagePtr &pError, const std::shared_ptr<Resources> & /*pDependencies*/) override {
		auto request = std::dynamic_pointer_cast<MyUniversalRequest>(pRequest);
		
		if (!request) {
			pError->setStacktrace();
			pError->setTitle("Failed to load a resource");
			pError->setMessage("Invalid request");
			return nullptr;
		}
		
		//Processing a custom data
		
		if (!request->isFoo()) {
			pError->setStacktrace();
			pError->setTitle("Failed to load a resource");
			pError->setMessage("Foo is false");
			pError->addInfoLine("Resource name: {}", request->getName());
			return nullptr;
		}
		auto result = std::make_shared<MyResource>();
		result->setBar("Lorem ipsum");
		return result;
	}
};
```

### Предопределенные запросы

SDK предоставляет некоторое количество предопределенных запросов.

- Шейдеры
	- Вершинные шейдеры
		- `mer::sdk::BuiltInVertexShaderRequest::getDefault`
	- Фрагментные шейдеры
		- `mer::sdk::BuiltInFragmentShaderRequest::getDefault`
	- Шейдерные программы

### Регистрация запроса

Для того чтобы запрос получил возможность быть исполненным, его необходимо зарегистрировать.

Процесс регистрации происходит перед загрузкой сцены в методе `mer::sdk::IScene::preloadScene`. Первый
параметр ---
это [класс](#mer::sdk::ResourceRequests), используя который, можно зарегистрировать запрос.

Перегрузка с шаблоном позволяет не создавать вручную экземпляр запроса:

```c++
using namespace mer::sdk;
ResourceRequests::addRequest<MyUniversalRequest>(false);
//                                   ^             ^
//                                ClassT         Args...
```

`ClassT` указывает на класс, экземпляр которого нужно создать.
`Args...` --- параметры, необходимые конструктору класса для создания экземпляра.

В случае, если экземпляр класса уже имеется и он обернут в std::shared_ptr (используя std::make_shared, например), то
можно его передать напрямую:

```c++
std::shared_ptr<MyUniversalRequest> myRequest = std::make_shared<MyUniversalRequest>(false);
//или
std::shared_ptr<MyUniversalRequest> myRequest = std::shared_ptr<MyUniversalRequest>(new MyUniversalRequest(false));

mer::sdk::ResourceRequests::addRequest(myRequest);
```

Рекомендуется использовать статичные запросы, так, как это показано в примере в конце статьи. Это позволит удобно
получать загруженные ресурсы.

### Процесс обработки запросов

\todo описать процесс исполнения зависимых запросов.

Движок хранит все ранее загруженные ресурсы. Это необходимо для уменьшения количества повторных загрузок одинаковых
ресурсов. Во время загрузки сцены, исполняются обычные запросы, которые ранее не были исполнены. На этом этапе все
требуемые обычные ресурсы загружены и они добавляются во внутренний список всех ресурсов движка. После этого формируется
список ресурсов на основе списка запросов. Ленивые ресурсы оборачиваются в `mer::sdk::LazyResource`, обычные же
добавляются как есть.

\warning Несоответствие политик загрузки ранее загруженных и загружаемых ресурсов приводит к подобным сообщениям об
ошибке:

\warning \code
Resource loading policy mismatch
Message: Resource previously loaded with LAZY policy, but trying to load with REGULAR
Additional info:
Current errno status: Resource temporarily unavailable
DO NOT CHANGE POLICY FOR SINGLE RESOURCE
Resource added as LAZY
Resource name: __BuiltIn_DefaultFragment__
\endcode

\warning Для их избежания, не делайте запросы с разными политиками для одного ресурса. Ранее загруженный ресурс не имеет
смысла
запрашивать лениво, так как он уже загружен. Как и не имеет смысла загружать ленивым запросом ресурс, если он в
дальнейшем будет использоваться как обычный ресурс.

## Загруженные ресурсы

После загрузки сцены, сформированный список ресурсов передается в экземпляр сцены. Получить ресурсы можно используя
методы класса `mer::sdk::Resources`, экземпляр которого можно получить,
вызвав `mer::sdk::Scene::getResources`
или же алиасы, объявленные в базовом классе сцены:

1. `mer::sdk::Scene::getResource`
2. `mer::sdk::Scene::getResourceByRequest`
3. `mer::sdk::Scene::getLazyResource`
4. `mer::sdk::Scene::getLazyResourceSync`
5. `mer::sdk::Scene::getLazyResourceAsync`
6. `mer::sdk::Scene::getLazyResourceFuture`

Метод 2 можно использовать для получения ресурса по запросу.

Методы 3-6 используются в случае, если получаемый ресурс - ленивый. Подробное описание находится ниже.

Пример получения загруженных ресурсов приведен в конце статьи.

### Ленивые ресурсы

В момент получения таких ресурсов, проверятся, загружено ли содержимое. Если оно загружено, то сразу же возвращается.
Если не загружено, то происходит загрузка синхронно или асинхронно, в зависимости от метода, который используется для
получения содержимого ленивого ресурса.

Если известно, что ленивый ресурс загружен, то нет необходимости использовать методы асинхронного получения ресурса, для
избежания накладных расходов.

Как говорилось ранее, существует несколько методов класса, позволяющих получить ленивый ресурс.

Метод `mer::sdk::Scene::getLazyResource` возвращает умный указатель на экземпляр `mer::sdk::LazyResource` по
имени ресурса из запроса. Используя этот метод, можно передать ленивый ресурс в другой метод без извлечения содержимого.
Класс ленивого ресурса имеет методы для загрузки и получения содержимого ресурса:

1. `mer::sdk::LazyResource::getAsync()`. Обе перегрузки позволяют загрузить и получить ресурс асинхронно, т.е. без
   блокирования потока выполнения. После загрузки будет вызвана функция, переданная в качестве параметра.
2. `mer::sdk::LazyResource::getSync()`. То же, что метод выше, только загрузка выполняется синхронно, т.е. с
   блокированием потока выполнения. Этот метод вернет ресурс после загрузки.
3. `mer::sdk::LazyResource::get()`. Обе перегрузки действуют также, как и getAsync(), но позволяют получить
   результат в потоке выполнения, откуда и был вызван метод. Подробнее
   про [std::future](https://en.cppreference.com/w/cpp/thread/future)

Остальные методы, упомянутые ранее --- синонимы к методам выше. Соответствие синонимов приведено в таблице ниже.

| Метод LazyResource | Метод Scene             |
|--------------------|-------------------------|
| getAsync()         | getLazyResourceAsync()  |
| getSync()          | getLazyResourceSync()   |
| get()              | getLazyResourceFuture() |

### Дальнейшее использование ресурсов

\todo Описать возможность использования загруженных пользовательских ресурсов и предопределенных движком.

## Пример реализации запроса, загрузчика и получения ресурса

```c++
//Предположим, у нас есть задача сделать универсальный запрос для загрузки разных шейдеров.
//Перечислим их:
enum class Shaders{
	LIGHT_VERTEX_SHADER,
	LIGHT_FRAGMENT_SHADER,
	POSTPROCESSING_VERTEX_SHADER
	...
};

// Класс запроса
class UniversalShaderRequest : public mer::sdk::ResourceRequest {
	std::string name;
	
	Shader shader; //Информация о шейдере, который необходимо загрузить
public:
	explicit UniversalShaderRequest(const std::string &pName, Shader pShader) : name(pName), shader(pShader) {}
	
	[[nodiscard]] std::shared_ptr<mer::sdk::ResourceLoader> getLoader() const override{
	 	//Указываем загрузчик, который будет заниматься загрузкой выбранного шейдера
		return std::make_shared<UniversalShaderLoader>();
	}

	[[nodiscard]] std::string getName() const override { return name; }

	//Метод для получения требуемого шейдера. Используется в UniversalShaderLoader
	[[nodiscard]] Shader getShader() const { return shader; }
};

//Класс загрузчика
class UniversalShaderLoader : public mer::sdk::ResourceLoader {
public:
	std::shared_ptr<mer::sdk::IResource> load(const std::shared_ptr<mer::sdk::ResourceRequest> &pRequest,
									const mer::sdk::ReportMessagePtr &pError) override {
		auto request = std::dynamic_pointer_cast<UniversalShaderRequest>(pRequest);
		using namespace mer::sdk::renderer;
		
		if (!request) {//Был передан неизвестный запрос, ошибка
			pError->setStacktrace();
			pError->setTitle("Failed to load a shader");
			pError->setMessage("Invalid request");
			return nullptr;
		}
		
		std::shared_ptr<Shader> result;
		//Загружаем исходный код выбранного шейдера
		switch (request->getShader()){
			case LIGHT_VERTEX_SHADER:
				result = std::make_shared<VertexShader>();
				//Можно загрузить из строки
				result->setSource(R"(SOME SHADER CODE)");
				break;
			case LIGHT_FRAGMENT_SHADER:
				result = std::make_shared<FragmentShader>();
				
				//А можно из файла
				result->setSource(loadFromFile("<filename>"));
				break;
			...
			default:
				//Неизвестный шейдер
				pError->setStacktrace();
				pError->setTitle("Failed to compile the shader");
				pError->setMessage("Unknown shader in request");
				pError->addInfoLine("Shader name: {}", request->getName());
				pError->addInfoLine("Shader info: {}", request->getShader());
				return nullptr;
		}
		
		//Компиляция
		result->compile();
		if (!result->getCompileStatus()) {//Ошибка компиляции
			pError->setStacktrace();
			pError->setTitle("Failed to compile the shader");
			pError->setMessage("Error in shader code detected");
			pError->addInfoLine("Shader name: {}", request->getName());
			pError->addInfoLine("Shader info: {}", request->getShader());
			std::string log;
			result->getInfoLog(log);
			pError->addInfoLine("Compiler log: {}", log);
			return nullptr;
		}
		return result;
	}
};

class FooScene : public mer::sdk::Scene {
public:
	mer::sdk::ReportMessagePtr preloadScene(
		const std::shared_ptr<mer::sdk::ResourceRequests> &pRequests) override{
			pRequests->addRequestLazy<UniversalShaderLoader>("LightVertexShader", LIGHT_VERTEX_SHADER);
			pRequests->addRequest<UniversalShaderLoader>("SomeShader", LIGHT_FRAGMENT_SHADER);
			pRequests->addRequest<UniversalShaderLoader>("SomeOtherShader", POSTPROCESSING_VERTEX_SHADER);
	}
	
	
	mer::sdk::ReportMessagePtr initScene() {
		using namespace mer::sdk::renderer;
		auto lightVertexShader = getResource<VertexShader>("LightVertexShader");
		
		auto someShader = getLazyResourceSync<FragmentShader>("SomeShader");
		
		//														Из документации по sigc++
		//																\/
		getLazyResourceAsync<FragmentShader>("SomeOtherShader", sigc::mem_fun(*this, &FooScene::onSomeOtherShaderLoaded));
		
		return nullptr;
	}
	
	//После загрузки ресурса "SomeOtherShader" будет вызван этот метод
	void onSomeOtherShaderLoaded(std::shared_ptr<mer::sdk::renderer::FragmentShader> pResource){
		...
	}
};
```
