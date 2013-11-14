/*
   Get data from server may receive timeout, when app enters to foreground, need to get these data from server again. But need to check if data is already there. So I abstract this class.
 */


@interface HttpModel : NSObject
@property (nonatomic, retain) id data;
- (void)loadData;
@end

@interface HttpGetModel : HttpModel
@end

@interface HttpGetDictModel : HttpGetModel
@end

@interface HttpGetArrayModel : HttpGetModel
@end

@interface LoginHttpModel : HttpModel
@end

@interface CurrenciesHttpModel : HttpGetArrayModel
@end

@interface CategoriesHttpModel : HttpGetArrayModel
@end

@interface CountriesHttpModel : HttpGetArrayModel
@end

@interface FollowersHttpModel : HttpGetArrayModel
@end

@interface FollowingHttpModel : HttpGetDictModel
@end

@interface MyStoresHttpModel : HttpGetArrayModel
@end

@interface LikedDealsHttpModel : HttpGetArrayModel
@end

