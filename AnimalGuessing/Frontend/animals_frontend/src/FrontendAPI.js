export default class FrontendAPI {
    constructor(serviceAddress) {
        this.serviceAddress = serviceAddress
    }

    makeURL(operation, text) {
        const resource = operation + '/' + text
        return new URL(resource, this.serviceAddress);
    }

    sendStr(operation, text, handler) {
        fetch(this.makeURL(operation, text))
            .then(res => res.json())
            .then((response)=>{
                handler(response["expectation"], response["response"]);
            },
            (err) => {
                handler(err);
                console.log(err)
            });
    }
}