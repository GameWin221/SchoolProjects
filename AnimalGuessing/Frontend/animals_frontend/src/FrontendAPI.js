export default class FrontendAPI {
    constructor(serviceAddress) {
        this.serviceAddress = serviceAddress
    }

    makeURL(str) {
        return new URL(str, this.serviceAddress);
    }

    sendStr(str, handler) {
        fetch(this.makeURL(str))
            .then(res => res.json())
            .then((response)=>{
                handler(response["result"]);
            },
            (err) => {
                handler(err);
                console.log(err)
            });
    }
}